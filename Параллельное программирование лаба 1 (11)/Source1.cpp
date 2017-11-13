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
	//cout << "generated massive is" << endl << endl;

	for (int i = 0;i<len;i++)
	{
		mas[i] = rand() % 10;
		//cout << mas[i] << "  ";
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
bool compare(int *first, int *second,int length)
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

	return res;
}
void MPI_SCATTER(int *sbuf, int scount, MPI_Datatype stype, int *rbuf, int rcount, MPI_Datatype rtype, int root, MPI_Comm comm)
{
	int mpi_rank, mpi_size;
	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
	//if (mpi_rank == root)
	//{
		for (int i = 0; i < mpi_size; i++) //�� ���� �� ���� ��������� ���������
		{
			if (i == root)

			{
				//cout << root*rcount << endl;
				int count = 0;
				for (int i = root*rcount;i < (root + 1)*rcount;i++)
				{
					rbuf[count] = sbuf[i];
					count++;
				}
				continue;
			}
			MPI_Send(&sbuf[i*(rcount)], rcount, MPI_INT, i, 0, MPI_COMM_WORLD);
			cout << "Block of data was sent to process: " << i << endl;
		/*}*/
	}
}

int main(int argc,char** argv)
{
	MPI_Init(&argc, &argv);            //�������������
	int mpi_rank, mpi_size;            //���������� ���-�� ��������� � ����� ��������
	int mpi_root;
	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank); 
	MPI_Comm_size(MPI_COMM_WORLD, &mpi_size); 
	int *massive;                //������� �����
	int *tmpres;                //���� ����� ��� �������� ���� ����� ��� ���������� (��� �����) �������
	int *result;                //�������������� ������ � ������������
	int count = 0;                     //�� ���� �� ���� ��� ��������,� ���� ������ ����������
	int coeff = 0;                     //���������� �����,���������� ������ ��������
	MPI_Status Status;                 //���������� ������� ���������� �������� ������ ������
	int length = atoi(argv[1]);
	mpi_root = 3;


	coeff = length / mpi_size;		//�������� ������ ����� ������,������� �� ����� ����������
	int *tmp2 = new int[coeff];
	tmpres = new int[coeff];           //���� ����� ��� �������� ���� ����� ��� ���������� (��� �����) �������
	massive = new int[length];    //�������� ����������� ������ ��� �������
	result = new int[length];          //�������� ������ ��� ��������������� �������


	if (mpi_rank == mpi_root)                 //���� � ������� ��������
	{
		
		cout << "mpi_size=" << mpi_size << endl; //����� ���-�� ����������� ��������� � ���������
		cout << endl;
		make_matrix(massive, length);  //��������� 
		let_me_see(massive, length);	//������� �������
		MPI_SCATTER(massive, length, MPI_INT, tmp2, coeff, MPI_INT, mpi_root, MPI_COMM_WORLD);
	}

	else            //���� �� � ������� ��������
	{
		MPI_Recv(tmp2, coeff, MPI_INT, mpi_root, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //��������� � ����� tum ������ ������ �� �������� ��������
		cout << "Block of data was delivered to process: " << mpi_rank << endl;
		cout << "This block is: " << endl;
		let_me_see(tmp2, coeff);		
	}

	MPI_Gather(tmp2, coeff, MPI_INT, result, coeff, MPI_INT, mpi_root, MPI_COMM_WORLD);	
	
	if (mpi_rank == mpi_root) //���� � ������� ��������
	{
		let_me_see(result, length);
		if (compare(massive, result, length) == true)
		{
			cout << "equal";
		}
		else cout << "not equal";
	}

	  delete massive, result, tmpres; //������� ������
	  delete tmp2;
	  MPI_Finalize(); //��������� ������ � MPI
	  return 0;
}
