#include <iostream>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cstdlib>

#define MPI_TAG 0


using namespace std;
void make_matrix(int *mas,  int len)
{
	cout << "generated massive is" << endl << endl;

	for (int i = 0;i<len;i++)
	{
		mas[i] = rand() % 10;
		cout << mas[i] << "  ";
	}
}

void let_me_see(int *mas, int len) 
{   
	
	for (int i = 0; i < len; i++)
	{
		cout << mas[i] << "  ";
	}
	cout << endl;
}
bool compare(int *posl, int *parallel,int height)
{
	bool res = true;
	//cout << "height:" << height << endl;
	for (int i = 0;i < height;i++)
	{
		//cout << posl[i] << "?" << parallel[i] << endl;
		if (posl[i] != parallel[i])
		{
			res = false;
			break; 
		}
	}

	return res;
}

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);            //инициализаци€
	int mpi_rank, mpi_size;            //переменные кол-ва процессов и ранга процесса
	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank); 
	MPI_Comm_size(MPI_COMM_WORLD, &mpi_size); 
	int *massive;                //матрица Ќ”∆Ќј
	int *tmpres;                //сюда пишут все процессы свои суммы дл€ нескольких (или одной) строчек
	int *result;                //результирующий вектор в ѕј–јЋЋ≈Ћ№Ќќ…
	int count = 0;                     //не надо на мен€ так смотреть,€ ведь просто переменна€
	int coeff = 0;                     //количество строк,выдел€емых одному процессу
	double times;                      //переменные дл€ отображени€ времени работы
	MPI_Status Status;                 //переменна€ статуса выполнени€ операции приема данных
	int length = atoi(argv[1]);


	coeff = length / mpi_size;		//получаем размер блока данных,который мы будем передавать
	int *tmp2 = new int[coeff];
	tmpres = new int[coeff];           //сюда пишут все процессы свои суммы дн€ нескольких (или одной) строчек
	massive = new int[length];    //выдел€ем необходимую пам€ть дл€ матрицы
	result = new int[length];          //выдел€ем пам€ть дл€ результирующего вектора
	if (mpi_rank == 0)                 //если в нулевом процессе
	{
		cout << "mpi_size=" << mpi_size << endl; //вывод кол-ва действующих процессов в программе
		cout << endl;
		make_matrix(massive, length);  //заполн€ем и выводим матрицу
		tmpres = massive;
		for (int i = 1; i < mpi_size; i++) //мы идем по всем ненулевым процессам
			{
				MPI_Send(&massive[i*(coeff)], coeff, MPI_INT, i, 0, MPI_COMM_WORLD);
				cout << "Block of data was sent to process: " << i << endl;
			}
			cout << "Block of data was delivered to process: " << mpi_rank << endl;
			cout << "This block is: " << endl;
	}
		
	
	else            //если не в нулевом процессе
	{
	    int *tmp2 = new int[coeff];
		MPI_Recv(tmp2, coeff, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //принимаем в буфер tum нужную строку из нулевого процесса
		cout << "Block of data was delivered to process: " << mpi_rank << endl;
		cout << "This block is: " << endl;
		let_me_see(tmp2, coeff);
		tmpres = tmp2;
		delete tmp2;
		
	}
	MPI_Gather(tmpres, coeff, MPI_INT, result, coeff, MPI_INT, 0, MPI_COMM_WORLD);	
	
	if (mpi_rank == 0) //если в нулевом процессе
	{
		let_me_see(result, length);
		if (compare(massive, result, length) == true)
		{
			cout << "equal";
		}
		else cout << "not equal";
	}

	  delete massive, result, tmpres; //очищаем пам€ть
	  delete tmp2;
	  MPI_Finalize(); //завершаем работу с MPI
	  return 0;
}
