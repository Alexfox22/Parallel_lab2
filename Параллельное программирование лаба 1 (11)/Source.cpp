//#define MSMPI_NO_DEPRECATE_20
//#include <iostream>
//#include <mpi.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <time.h>
//#include <cstdlib>
//
//#define MPI_TAG 0
//#define max_value 10
//
//
//using namespace std;
//void make_matrix(double *mas, int len)
//{
//
//	for (int i = 0;i<len;i++)
//	{
//		//mas[i] = rand() % max_value;
//		mas[i] = ((double)rand() / RAND_MAX) * 10;
//	}
//}
//
//void let_me_see(double *mas, int len)
//{
//
//	for (int i = 0; i < len; i++)
//	{
//		cout << mas[i] << "  ";
//	}
//	cout << endl;
//}
//
//bool compare(double *first, double *second, int length)
//{
//	bool res = true;
//	for (int i = 0;i < length;i++)
//	{
//		if (first[i] != second[i])
//		{
//			res = false;
//			break;
//		}
//	}
//
//	return res;
//
//}
//void MPI_Scatter(void *sbuf, int scount, MPI_Datatype stype, void *rbuf, int rcount, MPI_Datatype rtype, int root, MPI_Comm comm)
//{
//	int mpi_rank, mpi_size;
//	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
//	MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
//	MPI_Aint sendExtent;
//	MPI_Aint recvExtent;
//
//	MPI_Type_extent(stype, &sendExtent);
//	MPI_Type_extent(rtype, &recvExtent);
//	MPI_Status status;
//	if (mpi_rank == root)
//	{
//
//		for (int i = 0; i < rcount * sendExtent; i++)
//			*((char *)(rbuf)+i) = *((char *)(sbuf)+root*rcount*sendExtent + i);
//		for (int i = 0; i < mpi_size; i++)
//		{
//			if (i == mpi_rank) continue;
//			MPI_Send((char *)sbuf + i * rcount * sendExtent, rcount, stype, i, 0, comm);
//		}
//	}
//
//	else
//	{
//		MPI_Recv(rbuf, rcount, rtype, root, 0, comm, &status);
//	}
//}
//
//
//int main(int argc, char** argv)
//{
//	MPI_Init(&argc, &argv);            //инициализация
//	int mpi_rank, mpi_size;            //переменные кол-ва процессов и ранга процесса
//	int mpi_root;
//	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
//	MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
//	double *massived;                //исходный массив
//	double *resultd;                //результирующий массив
//	int count = 0;                     //не надо на меня так смотреть,я ведь просто переменная
//	MPI_Status Status;                 //переменная статуса выполнения операции приема данных
//	int length = atoi(argv[1]);
//	mpi_root = atoi(argv[2]);
//	bool isProp = true;
//	srand(time(0));
//
//	int coeff = length / mpi_size;		//получаем размер блока данных,который мы будем передавать
//	double *tmp2 = new double[coeff];
//	massived = new double[length];    //выделяем необходимую память для маcсива
//	resultd = new double[length];     //выделяем память для результирующего массива
//	double startTime, endTime;
//
//	if (mpi_rank == mpi_root)                 //если в root
//	{
//		startTime = MPI_Wtime();
//		cout << "mpi_size=" << mpi_size << endl; //вывод кол-ва действующих процессов в программе
//		cout << endl;
//		make_matrix(massived, length);			//заполняем 
//		if (length<10)
//			let_me_see(massived, length);			//выводим массив	
//	}
//	MPI_Scatter(massived, length, MPI_DOUBLE, tmp2, coeff, MPI_DOUBLE, mpi_root, MPI_COMM_WORLD);  //раздаем из рута по кусочку каждому процессу
//
//	MPI_Gather(tmp2, coeff, MPI_DOUBLE, resultd, coeff, MPI_DOUBLE, mpi_root, MPI_COMM_WORLD);
//
//	if (mpi_rank == mpi_root) //если в нулевом процессе
//	{
//		endTime = MPI_Wtime();
//		if (length < 10)
//		{
//			cout << "Returned massive is: " << endl;
//			let_me_see(resultd, length);
//		}
//		if (compare(massived, resultd, length) == true)
//		{
//			cout << "equal" << endl;;
//		}
//		else cout << "not equal" << endl;
//		cout << "Time spended: " << endTime - startTime << endl;
//	}
//
//	delete massived, resultd; //очищаем память
//	delete tmp2;
//	MPI_Finalize(); //завершаем работу с MPI
//	return 0;
//}
