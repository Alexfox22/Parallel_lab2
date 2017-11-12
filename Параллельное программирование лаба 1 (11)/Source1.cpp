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
	MPI_Init(&argc, &argv);            //�������������
	int mpi_rank, mpi_size;            //���������� ���-�� ��������� � ����� ��������
	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank); 
	MPI_Comm_size(MPI_COMM_WORLD, &mpi_size); 
	int *massive;                //������� �����
	int *tmpres;                //���� ����� ��� �������� ���� ����� ��� ���������� (��� �����) �������
	int *result;                //�������������� ������ � ������������
	int count = 0;                     //�� ���� �� ���� ��� ��������,� ���� ������ ����������
	int coeff = 0;                     //���������� �����,���������� ������ ��������
	double times;                      //���������� ��� ����������� ������� ������
	MPI_Status Status;                 //���������� ������� ���������� �������� ������ ������
	int length = atoi(argv[1]);


	coeff = length / mpi_size;		//�������� ������ ����� ������,������� �� ����� ����������
	int *tmp2 = new int[coeff];
	tmpres = new int[coeff];           //���� ����� ��� �������� ���� ����� ��� ���������� (��� �����) �������
	massive = new int[length];    //�������� ����������� ������ ��� �������
	result = new int[length];          //�������� ������ ��� ��������������� �������
	if (mpi_rank == 0)                 //���� � ������� ��������
	{
		cout << "mpi_size=" << mpi_size << endl; //����� ���-�� ����������� ��������� � ���������
		cout << endl;
		make_matrix(massive, length);  //��������� � ������� �������
		tmpres = massive;
		for (int i = 1; i < mpi_size; i++) //�� ���� �� ���� ��������� ���������
			{
				MPI_Send(&massive[i*(coeff)], coeff, MPI_INT, i, 0, MPI_COMM_WORLD);
				cout << "Block of data was sent to process: " << i << endl;
			}
			cout << "Block of data was delivered to process: " << mpi_rank << endl;
			cout << "This block is: " << endl;
	}
		
	
	else            //���� �� � ������� ��������
	{
	    int *tmp2 = new int[coeff];
		MPI_Recv(tmp2, coeff, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //��������� � ����� tum ������ ������ �� �������� ��������
		cout << "Block of data was delivered to process: " << mpi_rank << endl;
		cout << "This block is: " << endl;
		let_me_see(tmp2, coeff);
		tmpres = tmp2;
		delete tmp2;
		
	}
	MPI_Gather(tmpres, coeff, MPI_INT, result, coeff, MPI_INT, 0, MPI_COMM_WORLD);	
	
	if (mpi_rank == 0) //���� � ������� ��������
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
