#include <iostream>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cstdlib>

#define MPI_TAG 0
#define max_value 10

using namespace std;
void make_matrix(double *mas,  int len)
{

	for (int i = 0;i<len;i++)
	{
		mas[i] = rand() % max_value;
	}
}

void let_me_see(double *mas, int len) 
{   
	
	for (int i = 0; i < len; i++)
	{
		cout << mas[i] << "  ";
	}
	cout << endl;
}

bool compare(double *first, double *second,int length)
{
	bool res = true;
	for (int i = 0;i < length;i++)
	{	
		if (first[i] != second[i])
		{
			res = false;
			break; 
		}
	}

	//MPI_Scatter();
	return res;

}
void MPI_Scatter(double *sbuf, int scount, MPI_Datatype stype, double *rbuf, int rcount, MPI_Datatype rtype, int root, MPI_Comm comm)
{
	int mpi_rank, mpi_size;
	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

		for (int i = 0; i < mpi_size; i++) 
		{
			if (i == root)

			{
				int count = 0;
				for (int i = root*rcount;i < (root + 1)*rcount;i++)
				{
					rbuf[count] = sbuf[i];
					count++;
				}
				continue;
			}
		
			MPI_Send(&sbuf[i*(rcount)], rcount, stype, i, 0, comm);
		}
		cout << "Data was send" << endl;
}

int main(int argc,char** argv)
{
	MPI_Init(&argc, &argv);            //�������������
	int mpi_rank, mpi_size;            //���������� ���-�� ��������� � ����� ��������
	int mpi_root;
	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank); 
	MPI_Comm_size(MPI_COMM_WORLD, &mpi_size); 
	double *massived;                //�������� ������
	double *resultd;                //�������������� ������
	int count = 0;                     //�� ���� �� ���� ��� ��������,� ���� ������ ����������
	MPI_Status Status;                 //���������� ������� ���������� �������� ������ ������
	int length = atoi(argv[1]);
	mpi_root = atoi(argv[2]);
	bool isProp = true;

	int coeff = length / mpi_size;		//�������� ������ ����� ������,������� �� ����� ����������
	double *tmp2 = new double[coeff];
	massived = new double[length];    //�������� ����������� ������ ��� ��c����
	resultd = new double[length];     //�������� ������ ��� ��������������� �������
	double startTime, endTime;



	if (mpi_rank == mpi_root)                 //���� � root
	{
		startTime = MPI_Wtime();
		cout << "mpi_size=" << mpi_size << endl; //����� ���-�� ����������� ��������� � ���������
		cout << endl;
		make_matrix(massived, length);			//��������� 
		let_me_see(massived, length);			//������� ������
		MPI_Scatter(massived, length, MPI_DOUBLE, tmp2, coeff, MPI_DOUBLE, mpi_root, MPI_COMM_WORLD);  //������� �� ���� �� ������� ������� ��������
		
	}

	else									 //���� �� � root
	{
		//cout << "Process num: " << mpi_rank << " got data" << endl;
		MPI_Recv(tmp2, coeff, MPI_DOUBLE, mpi_root, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //��������� � ����� tum ������ ������ �� �������� ��������	
		
	}
	//MPI_Barrier(MPI_COMM_WORLD);
	MPI_Gather(tmp2, coeff, MPI_DOUBLE, resultd, coeff, MPI_DOUBLE, mpi_root, MPI_COMM_WORLD);	
	
	if (mpi_rank == mpi_root) //���� � ������� ��������
	{
		endTime = MPI_Wtime();
		cout << "Returned massive is: " << endl;
		let_me_see(resultd, length);
		if (compare(massived, resultd, length) == true)
		{
			cout << "equal";
		}
		else cout << "not equal";
		cout << "Time spended: " << endTime - startTime << endl;
	}

	  delete massived, resultd; //������� ������
	  delete tmp2;
	  MPI_Finalize(); //��������� ������ � MPI
	  return 0;
}
