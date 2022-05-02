#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <assert.h>
#include <signal.h>

// Playfield size
#define WIDTH 80
#define HEIGHT 25

typedef enum {
	UP, DOWN, LEFT, RIGHT
} direction;

// Coordinates + direction
typedef struct DirectedPoint {
	int row, col;
	direction dir;
} DirectedPoint;

// Struct that holds info about one snake.
typedef struct Snake {
	DirectedPoint head; // Both head and tail must be directed to move snake
	DirectedPoint tail;
	direction new_dir;
	unsigned long long length; // Current snake length. Maybe not needed
	long long lengthbuf; // If > 0, snake grows via lengthbuf--, length++
} Snake;

// Actual playfield size without walls
enum { width = WIDTH - 2,
       height = HEIGHT - 3 };

enum { KEY_ARROWU = 0x415b1b,
       KEY_ARROWD = 0x425b1b,
       KEY_ARROWR = 0x435b1b,
       KEY_ARROWL = 0x445b1b };

enum { BILLION = 1000000000 }; // Just a shortcut
enum { FOOD_RARITY = 512, // Read as 'One food per FOOD_RARITY empty cells'
       LVLUP_LENGTH = 50 }; // Level up each time this much length was gained

sig_atomic_t run = 1; // For SIGINT handling

struct termios saved_attributes;

unsigned char playfield[height][width];
int food_cnt = 0;
int level = 1;


void reset_input_mode(void){
	printf("\x1b[?25h");
	tcsetattr(STDIN_FILENO, TCSANOW, &saved_attributes);
	system("clear");
}


void sighandler(int sig){
	run = 0;
	signal(sig, sighandler);
}


void set_terminal_mode(void){
	struct termios tattr;

	setbuf(stdout, NULL);
	/* Make sure stdin is a terminal. */
	if (!isatty(STDIN_FILENO)){
		fprintf(stderr, "Not a terminal.\n");
		exit(EXIT_FAILURE);
	}

	/* Save the terminal attributes so we can restore them later. */
	tcgetattr(STDIN_FILENO, &saved_attributes);

	/* Set the non-conventional mode and disable echo so game controls can be
	 * implemented. */
	tcgetattr(STDIN_FILENO, &tattr);
	tattr.c_lflag &= ~(ICANON|ECHO); /* Clear ICANON and ECHO. */
	tattr.c_cc[VMIN] = 0;
	tattr.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &tattr);

	printf("\x1b[?25l");
	system("clear");
	atexit(reset_input_mode);
}

int turn_up(Snake *snake){
	DirectedPoint *head = &(snake->head);
	char bodyseg;
	int success = 1;

	switch(head->dir){
	case LEFT:
		bodyseg = '\\';
		break;
	case RIGHT:
		bodyseg = '/';
		break;
	case DOWN:
		success = 0; // Fall through intended
	default:
		goto ret;
	}

	snake->new_dir = UP;
	playfield[head->row][head->col] = bodyseg;

  ret:
	return success;
}

int turn_down(Snake *snake){
	DirectedPoint *head = &(snake->head);
	char bodyseg;
	int success = 1;

	switch(head->dir){
	case LEFT:
		bodyseg = '/';
		break;
	case RIGHT:
		bodyseg = '\\';
		break;
	case UP:
		success = 0; // Fall through intended
	default:
		goto ret;
	}

	snake->new_dir = DOWN;
	playfield[head->row][head->col] = bodyseg;

  ret:
	return success;
}

int turn_left(Snake *snake){
	DirectedPoint *head = &(snake->head);
	char bodyseg;
	int success = 1;

	switch(head->dir){
	case UP:
		bodyseg = '\\';
		break;
	case DOWN:
		bodyseg = '/';
		break;
	case RIGHT:
		success = 0; // Fall through intended
	default:
		goto ret;
	}

	snake->new_dir = LEFT;
	playfield[head->row][head->col] = bodyseg;

  ret:
	return success;
}

int turn_right(Snake *snake){
	DirectedPoint *head = &(snake->head);
	char bodyseg;
	int success = 1;

	switch(head->dir){
	case UP:
		bodyseg = '/';
		break;
	case DOWN:
		bodyseg = '\\';
		break;
	case LEFT:
		success = 0; // Fall through intended
	default:
		goto ret;
	}

	snake->new_dir = RIGHT;
	playfield[head->row][head->col] = bodyseg;

  ret:
	return success;
}

// A function to do different things based on key that was pressed.
// Returns 1 if snake needs to be sped up on this tick
int process_key(Snake *snake){
	int c = 0;
	int snake_moved = 0;

	if(read(STDIN_FILENO, &c, 3)){
		switch(c){
		case KEY_ARROWU:
			snake_moved = turn_up(snake);
			break;
		case KEY_ARROWD:
			snake_moved = turn_down(snake);
			break;
		case KEY_ARROWL:
			snake_moved = turn_left(snake);
			break;
		case KEY_ARROWR:
			snake_moved = turn_right(snake);
			break;
		}
	}

	// TODO: do something about several keys pressed at once.
	// And generally 'normal key' processing
	return snake_moved;
}

// A function to initialize the playfield. Playfield preparation is for now
// simple space filling
void init_playfield(void){
	for (int j = 0; j < height; j++){
		for(int i = 0; i < width; playfield[j][i++] = ' ');
	}
}

// A function to initialize a snake. For now starting coordinates are fixed,
// but if the game will ever be extended for multiplayer, this must change
void init_snake(Snake *snake){
	playfield[height / 2][width / 2] = '<';

	snake->head.row = height / 2;
	snake->head.col = width / 2;
	snake->head.dir = RIGHT;

	snake->tail.row = height / 2;
	snake->tail.col = width / 2;
	snake->tail.dir = RIGHT;

	snake->new_dir = RIGHT;

	snake->length = 1;
	snake->lengthbuf = 4; // Total starting length - 5
}

// A function to redraw playfield on screen with all changes at the end of the
// game move (tick)
void redraw_all(Snake *snake){
	printf("\x1b[H"); // Move cursor to 0,0 position

	for(int i = width + 2; i; i--){ // Draw top wall
		putchar('#');
	}
	putchar('\n');

	for(int j = 0; j < height; j++){ // Draw field
		putchar('#'); // And side walls
		for(int i = 0; i < width; i++){
			putchar(playfield[j][i]);
		}
		putchar('#'); // Side wall
		putchar('\n');
	}

	for(int i = width + 2; i; i--){ // Draw bottom wall
		putchar('#');
	}
	putchar('\n');

	printf("Length: %5lld\tLevel: %3d", snake->length, level);
}

// A function to redraw playfield on screen without any changes, but with
// different mouth character for snake. Simple-ish eating animation.
void redraw_animation(Snake *snake){
	// Move cursor to where snake's head is
	printf("\x1b[%d;%dH", snake->head.row + 2, snake->head.col + 2);

	if(snake->head.dir == UP || // Draw appropriate closed mouth
	   snake->head.dir == DOWN){
		putchar('|');
	} else {
		putchar('-');
	}

	// Move cursor to where snake's tail is
	printf("\x1b[%d;%dH", snake->tail.row + 2, snake->tail.col + 2);

	if(playfield[snake->tail.row][snake->tail.col] != '-' &&
	   playfield[snake->tail.row][snake->tail.col] != '|'){
		if(snake->tail.dir == UP ||
		   snake->tail.dir == DOWN){
			putchar('|');
		} else {
			putchar('-');
		}
	}
}

void move_snake(Snake *snake){
	DirectedPoint *head = &(snake->head);
	DirectedPoint *tail = &(snake->tail);
	unsigned char headchar;

	// If snake turned, new body segment was handled by turn function. If it
	// didn't turn, it has to be handled here
	if(head->dir == snake->new_dir){
		if(head->dir == UP || head->dir == DOWN){
			playfield[head->row][head->col] = '|';
		} else {
			playfield[head->row][head->col] = '-';
		}
	} else{
		head->dir = snake->new_dir;
	}

	// Move head DirectedPoint and choose appropriate character to represent head
	// facing current direction
	switch(head->dir){
	case UP:
		head->row--;
		headchar = 'V';
		break;
	case DOWN:
		head->row++;
		headchar = '^';
		break;
	case LEFT:
		head->col--;
		headchar = '>';
		break;
	case RIGHT:
		head->col++;
		headchar = '<';
		break;
	default:
		assert(0); // This should never happen
	}

	// Wall hit event
	if(head->col < 0 ||
	   head->col >= width ||
	   head->row < 0 ||
	   head->row >= height){
		reset_input_mode();
		system("clear");
		printf("You hit a wall. Game over!\n");
		printf("Press any key to exit\n");
		getchar();
		exit(0);
	}

	// Food eat event
	if(playfield[head->row][head->col] == '@'){
		snake->lengthbuf++;
		food_cnt--;
	}
	// Snake body hit event. Everything that's not food or space is snake
	else if(playfield[head->row][head->col] != ' '){
		reset_input_mode();
		system("clear");
		printf("You bit yourself. Game over!\n");
		printf("Press any key to exit\n");
		getchar();
		exit(0);
	}

	// Print head only after all possible gameover-events checked
	playfield[head->row][head->col] = headchar;

	// Move tail only if lengthbuf is 0. Otherwise, do not move tail - snake
	// grows 1 segment
	if(snake->lengthbuf){
		snake->lengthbuf--;
		snake->length++;
	} else {
		playfield[tail->row][tail->col] = ' ';

		// Move tail DirectedPoint
		switch(tail->dir){
		case UP:
			tail->row--;
			break;
		case DOWN:
			tail->row++;
			break;
		case LEFT:
			tail->col--;
			break;
		case RIGHT:
			tail->col++;
			break;
		default:
			assert(0); // This should never happen
		}

		// Determine new tail direction
		switch(playfield[tail->row][tail->col]){
		case '-': // Next body segment is straight - direction doesn't change
		case '|': // Fall through intended
			break;
		case '/': // Next body segment is turn type 1
			switch(tail->dir){ // New direction depends on old one
			case UP:
				tail->dir = RIGHT;
				break;
			case DOWN:
				tail->dir = LEFT;
				break;
			case LEFT:
				tail->dir = DOWN;
				break;
			case RIGHT:
				tail->dir = UP;
				break;
			default:
				assert(0); // This should never happen
			}
			break;
		case '\\': // Next body segment is turn type 2
			switch(tail->dir){ // New direction depends on old one
			case UP:
				tail->dir = LEFT;
				break;
			case DOWN:
				tail->dir = RIGHT;
				break;
			case LEFT:
				tail->dir = UP;
				break;
			case RIGHT:
				tail->dir = DOWN;
				break;
			default:
				assert(0); // This should never happen
			}
			break;
		default:
			assert(0); // This should never happen
		}
	}
}

// A function to randomly generate food on playfield
void gen_food(){
	static const int max_food = (width * height) / FOOD_RARITY + 1;
	int row, col;

	if(food_cnt <= max_food){
		row = rand() % height;
		col = rand() % width;
		if(playfield[row][col] == ' '){ // If generated position empty
			playfield[row][col] = '@'; // Put food there
			food_cnt++;
		} else {
			gen_food(); // Else try again
		}
	}
}

void level_up(Snake *snake){
	static int upped_already = 0;

	if(snake->length % LVLUP_LENGTH == 0){
		if(!upped_already){
			level++;
			upped_already = 1;
		}
	} else {
		upped_already = 0;
	}
}

int main(void){
	set_terminal_mode();
	signal(SIGINT, sighandler);
	srand(time(NULL));

	// These are needed for redraw timing
	struct timespec time_start, time_now;
	long long time_delta;

	char animation_frame = 1;

	Snake snake0;
	int snake_moved = 0;

	init_playfield();
	init_snake(&snake0);

	redraw_all(&snake0);

	clock_gettime(CLOCK_REALTIME, &time_start);

	while(run){
		snake_moved = process_key(&snake0);

		// This section ensures that redraw occurs on time. Billion because
		// nanoseconds are used.
		clock_gettime(CLOCK_REALTIME, &time_now);
		time_delta = ((time_now.tv_sec - time_start.tv_sec) * BILLION +
		              (time_now.tv_nsec - time_start.tv_nsec));
		if(snake_moved ||
		   time_delta > BILLION / (level + 1) / 5){
			time_start = time_now;
			if(animation_frame){
				redraw_animation(&snake0);
				animation_frame = 0;
			} else {
				snake_moved = 0;
				gen_food();
				level_up(&snake0);
				move_snake(&snake0);
				redraw_all(&snake0);
				animation_frame = 1;
			}
		}
	}

	return EXIT_SUCCESS;
}
