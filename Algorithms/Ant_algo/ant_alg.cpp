#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <algorithm>


#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1000
#define FRAMERATE 60
#define CITIES 20
#define COLOR_DENSITY 20
#define CONSOLE_OUTPUT 1
#define ITERATIONS 20

class AntAlgorithm
{
	template<typename T> using vector = std::vector<T>;

	struct CityType
	{
		int x, y;
	};

	struct AntType
	{
		int currentCity;
		int nextCity;
		vector<bool> usedCities;
		vector<int> path;
		double pathLength;

		void Restart(size_t newCity, int maxCities)
		{
			nextCity = -1;
			pathLength = 0.0;
			currentCity = newCity;
			path.clear();
			path.push_back(newCity);
			usedCities.clear();
			usedCities.resize(maxCities, false);
			usedCities[newCity] = true;
		}
	};

	double Alpha, Beta, RHO, Qvalue;
	double initPheromone;

	inline double getAntProduct(int from, int to)
	{
		return std::pow(pheromone[from][to], Alpha) * std::pow((1.0 / distance[from][to]), Beta);
	}

	int SelectNextCity(int ant)
	{
		int from = ants[ant].currentCity;
		double denom = 0.0;
		size_t to = 0;

		for (to = 0; to < maxCities; to++)
		{
			if (!ants[ant].usedCities[to])
			{
				denom += getAntProduct(from, to);
			}
		}

		while (true)
		{
			double p;
			to++;

			if (to >= maxCities) to = 0;

			if (!ants[ant].usedCities[to])
			{
				p = getAntProduct(from, to) / denom;

				if (((double)rand() / (double)RAND_MAX) < p) break;
			}
		}

		return to;
	}

public:
	int maxCities, maxDistance, maxAnts;
	double bestPath;
	int bestAnt;

	vector<CityType> cities;
	vector<AntType> ants;
	vector<vector<double> > distance, pheromone;

	AntAlgorithm(int cities, int distance, int ants) : maxCities(cities), maxDistance(distance), maxAnts(ants)
	{
		this->initPheromone = 1.0 / maxCities;
		this->cities.resize(maxCities);
		this->ants.resize(maxAnts);
		this->distance.resize(maxCities, vector<double>(maxCities));
		this->pheromone.resize(maxCities, vector<double>(maxCities, initPheromone));
		this->bestPath = maxDistance * maxCities;
	}

	void Ininitialize(double Alpha, double Beta, double RHO, double Qvalue)
	{
		this->Alpha = Alpha;
		this->Beta = Beta;
		this->RHO = RHO;
		this->Qvalue = Qvalue;

		for (size_t i = 0; i < maxCities; i++)
		{
			cities[i].x = rand() % maxDistance;
			cities[i].y = rand() % maxDistance;
		}

		for (size_t from = 0; from < maxCities; from++)
		{
			for (size_t to = 0; to < maxCities; to++)
			{
				if (to != from && distance[to][from] == 0.0)
				{
					int dx = cities[to].x - cities[from].x;
					int dy = cities[to].y - cities[from].y;
					distance[to][from] = std::sqrt(dx * dx + dy * dy);
					distance[from][to] = distance[to][from];
				}
			}
		}

		size_t city = 0;
		for (size_t ant = 0; ant < maxAnts; ant++, city++)
		{
			if (city == maxCities) city = 0;

			ants[ant].Restart(city, maxCities);
		}
	}

	void RestartAnts()
	{
		size_t city = 0;
		for (size_t ant = 0; ant < maxAnts; ant++, city++)
		{
			if (ants[ant].pathLength < bestPath)
			{
				bestPath = ants[ant].pathLength;
				bestAnt = ant;
			}
			if (city == maxCities) city = 0;

			ants[ant].Restart(city, maxCities);
		}
	}

	int SimulateAnts()
	{
		int moving = 0;
		for (size_t ant = 0; ant < maxAnts; ant++)
		{
			if (ants[ant].path.size() < maxCities)
			{
				int nextCity = SelectNextCity(ant);
				ants[ant].nextCity = nextCity;
				ants[ant].usedCities[nextCity] = true;
				ants[ant].path.push_back(nextCity);
				ants[ant].pathLength += distance[ants[ant].currentCity][nextCity];

				if (ants[ant].path.size() == maxCities)
				{
					ants[ant].pathLength += distance[ants[ant].path.back()][ants[ant].path.front()];
				}

				ants[ant].currentCity = nextCity;

				moving++;
			}
		}
		return moving;

	}

	void UpdateTrails()
	{
		for (size_t from = 0; from < maxCities; from++)
		{
			for (size_t to = 0; to < maxCities; to++)
			{
				if (from != to)
				{
					pheromone[from][to] *= (1.0 - RHO);

					if (pheromone[from][to] < 0.0)
					{
						pheromone[from][to] = initPheromone;
					}
				}
			}
		}

		int from, to;

		for (size_t ant = 0; ant < maxAnts; ant++)
		{
			for (size_t city = 0; city < maxCities; city++)
			{
				from = ants[ant].path[city];
				to = ants[ant].path[(city + 1) % maxCities];

				pheromone[from][to] += (Qvalue / ants[ant].pathLength);
				pheromone[to][from] = pheromone[from][to];
			}
		}

		for (size_t from = 0; from < maxCities; from++)
		{
			for (size_t to = 0; to < maxCities; to++)
			{
				pheromone[from][to] *= RHO;
			}
		}
	}
};

int main()
{
	srand(time(NULL));
	sf::RenderWindow window(sf::VideoMode(WINDOW_HEIGHT, WINDOW_HEIGHT), "Ant Algorithm");
	window.setFramerateLimit(FRAMERATE);

	AntAlgorithm antAlgorithm(CITIES, WINDOW_HEIGHT, 10);
	antAlgorithm.Ininitialize(1.0, 5.0, 0.5, 100);

	sf::CircleShape cityShape(2, 10);
	cityShape.setFillColor(sf::Color::Yellow);

	int iters = 0;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();

		if (iters < ITERATIONS)
		{
			if (antAlgorithm.SimulateAnts() == 0)
			{
				iters++;
				if (iters != ITERATIONS)
				{
					antAlgorithm.UpdateTrails();
					antAlgorithm.RestartAnts();
				}
			}
			if (CONSOLE_OUTPUT)
			{
				std::cout << antAlgorithm.bestPath << std::endl;
				std::cout << antAlgorithm.bestAnt << ": ";
				for (int city : antAlgorithm.ants[antAlgorithm.bestAnt].path)
				{
					std::cout << city << " ";
				}
				std::cout << "\n" << std::endl;
			}

			for (int from = 0; from < antAlgorithm.maxCities; from++)
			{
				for (int to = 0; to < antAlgorithm.maxCities; to++)
				{
					int opacity = 255 * antAlgorithm.pheromone[from][to] * COLOR_DENSITY;
					sf::VertexArray line(sf::LineStrip, 2);
					sf::Color color(255, 0, 0, std::min(opacity, 255));
					line[0].position = sf::Vector2f(antAlgorithm.cities[to].x, antAlgorithm.cities[to].y);
					line[0].color = color;
					line[1].position = sf::Vector2f(antAlgorithm.cities[from].x, antAlgorithm.cities[from].y);
					line[1].color = color;
					window.draw(line);
				}
			}
		}
		else
		{
			auto ant = antAlgorithm.ants[antAlgorithm.bestAnt];
			for (int i = 0; i < ant.path.size(); i++)
			{
				sf::VertexArray line(sf::LineStrip, 2);
				sf::Color color(255, 0, 0);
				line[0].position = sf::Vector2f(antAlgorithm.cities[ant.path[i]].x, antAlgorithm.cities[ant.path[i]].y);
				line[0].color = color;
				line[1].position = sf::Vector2f(antAlgorithm.cities[ant.path[(i + 1) % ant.path.size()]].x, 
												antAlgorithm.cities[ant.path[(i + 1) % ant.path.size()]].y);
				line[1].color = color;
				window.draw(line);
			}
		}

		for (auto city : antAlgorithm.cities)
		{
			cityShape.setPosition(city.x, city.y);
			window.draw(cityShape);
		}

		window.display();
	}
	return 0;
}
