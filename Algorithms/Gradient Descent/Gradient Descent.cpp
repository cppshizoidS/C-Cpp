#include <iostream>
#include <cmath>

struct vector
{
   double x,y;
};
 

double fx(vector x)
{
   return 100*(x.x*x.x+x.y*x.y);
}

vector gradient(vector x)
{
   vector grad;
 
   grad.x=200*x.x;
   grad.y=200*x.y;
 
   return grad;
}
 
//Вычисление одномерной функции для нахождения шага методом золотого сечения
double MakeSimplefx(double x, vector grad, vector xj)
{
   vector buffer;
 
   buffer.x=xj.x-x*grad.x;
   buffer.y=xj.y-x*grad.y;
 
   return fx(buffer);
}
 

double GoldenSelection(double a, double b, double eps, vector gradient, vector x)
{
   const double fi=1.6180339887;
   double x1,x2;
   double y1,y2;
 
   x1=b-((b-a)/fi);
   x2=a+((b-a)/fi);
   y1=MakeSimplefx(x1,gradient,x);
   y2=MakeSimplefx(x2,gradient,x);
   while (std::abs(b-a)>eps)
   {
      if (y1<=y2)
      {
         b=x2;
         x2=x1;
         x1=b-((b-a)/fi);
         y2=y1;
         y1=MakeSimplefx(x1,gradient,x);
      }
      else
      {
         a=x1;
         x1=x2;
         x2=a+((b-a)/fi);
         y1=y2;
         y2=MakeSimplefx(x2,gradient,x);
      }
   }
 
   return (a+b)/2;
}
 

vector Calculate(vector x, vector gradient, double lambda)
{
   vector buffer;
 
   buffer.x=x.x-lambda*gradient.x;
   buffer.y=x.y-lambda*gradient.y;
 
   return buffer;
}
 
vector GradDown(vector x, double eps)
{
   vector current=x;
   vector last;
 
   do
   {
      last=current; //Запоминаем предыдущее значение
      vector grad=gradient(current); //Вычисляем градиент
      double lambda=GoldenSelection(0,0.05,eps,grad,current); //Находим шаг вычислений методом золотого сечения
      current=Calculate(current,grad,lambda); //Вычисляем новое приближение
   }while(std::abs(fx(current)-fx(last))>eps); //Проверяем условие
 
   return current;
}
 
int main()
{
   vector x;
   double eps;
 
   std::cout<<"Введите через пробел начальное приближение x и y (например: -1 1): ";
   std::cin>>x.x>>x.y;
   std::cout<<"\nВведите точность вычислений (например 0.000001): ";
   std::cin>>eps;
   vector result=GradDown(x,eps);
   std::cout<<"\nРезультат: x = "<<result.x<<" y = "<<result.y;
 
   return 0;
};
