#include <stdio.h>
#include <stdlib.h>

using namespace std;

bool isInArr(int numb, int *arr, int arr_sz);
void decToBin(int dec, int *bin, int size);
int binToDec(int *bin, int size);
void hamCo(char *srcFlName);
void hamDe(char *srcFlName);

int main()
{
	char srcFlName[] = "/Users/mac/Desktop/univer/proggin/oib/lab 10/text.txt";
	hamCo(srcFlName);
	char srcCodedFlName[] = "/Users/mac/Desktop/univer/proggin/oib/lab 10/ham_ham.txt";
	hamDe(srcCodedFlName);
	return 0;
}

bool isInArr(int numb, int * arr, int arr_sz)
{
	for (int i = 0; i < arr_sz; i++)
		if (arr[i] == numb)
			return true;
	return false;
}

void decToBin(int dec, int *bin, int size)
{
	for (int i = size - 1; i >= 0; i--)
	{
		bin[i] = dec % 2;
		dec >>= 1;
	}
	return;
}

int binToDec(int *bin, int size)
{
	int out = 0, temp = 1;
	for (int i = size - 1; i >= 0; i--)
	{
		out += bin[i] * temp;
		temp <<= 1;
	}
	return out;
}

void hamCo(char *srcFlName)
{
	int binDeg[7];
	binDeg[0] = 1;
	binDeg[1] = 2;
	binDeg[2] = 4;
	binDeg[3] = 8;
	binDeg[4] = 16;
	binDeg[5] = 32;
	binDeg[6] = 64;

	int block_size = 0;
	while(1)
	{
		printf("size of bit block (64 max) : ");
		scanf("%d", &block_size);
		if (block_size < 65 && block_size > 0)
			break;
		printf("wrong number\n");
	}
	int i = 0;
	while (block_size > binDeg[i])
		i++;
	int numb_of_degree = i + 1;
	block_size += numb_of_degree;
	
	int c;
	int flag = 0;
	char buffer[8];
	int *out_buffer = (int *)malloc(block_size * sizeof(int));
	
	FILE *src = fopen(srcFlName, "r");
	FILE *binFl = fopen("/Users/mac/Desktop/univer/proggin/oib/lab 10/bin.txt", "w+");
	FILE *ham = fopen("/Users/mac/Desktop/univer/proggin/oib/lab 10/ham_ham.txt", "w");
	while ((c = fgetc(src)) != EOF)
	{
		int bin[8] = { 0 };
		decToBin(c, bin, 8);
		for (int i = 0; i < 8; i++)
			fprintf(binFl, "%d", bin[i]);
	}
	fclose(src);
	fseek(binFl, 0, SEEK_SET);

	while (!feof(binFl))
	{
		for(int i = 0; i < block_size; i++)
			out_buffer[i] = 0;
		for(int i = 0; i < numb_of_degree; i++)
			out_buffer[binDeg[i] - 1] = -1;
		
		i = 0;
		while(i < block_size && (c = fgetc(binFl)) != EOF)
		{
			while(i < block_size && out_buffer[i] == -1)
				i++;
			if(i < block_size)
				out_buffer[i] = c - '0';
			i++;
		}

		if(i != block_size)
		{
			int t = 0;
			while (i > binDeg[t])
				t++;
			numb_of_degree = t;
			block_size = i;
		}
		for(int cnt = 0; cnt < numb_of_degree; cnt++)
		{
			int step = binDeg[cnt];
			int start = binDeg[cnt] - 1;
			int summ = 0;
			int j = start;
			while(j < block_size)
			{
				int t = j;
				if(j == start)
					t++;
				for(; t < block_size && t < j + step; t++)
					summ += out_buffer[t];
				j += step + step;
			}
			out_buffer[binDeg[cnt] - 1] = summ % 2;
		}
		for(int j = 0; j < i; j++)
			fprintf(ham, "%d", out_buffer[j]);
	}

	free(out_buffer);
	fclose(binFl);
	fclose(ham);
	return;
}

void hamDe(char *srcFlName)
{
	int binDeg[7];
	binDeg[0] = 1;
	binDeg[1] = 2;
	binDeg[2] = 4;
	binDeg[3] = 8;
	binDeg[4] = 16;
	binDeg[5] = 32;
	binDeg[6] = 64;
	int i;

	int block_size = 0;
	while(1)
	{
		printf("size of bit block (64 max) : ");
		scanf("%d", &block_size);
		if (block_size < 65 && block_size > 0)
			break;
		printf("wrong number\n");
	}
	i = 0;
	while (block_size > binDeg[i])
		i++;
	int numb_of_degree = i + 1;
	block_size += numb_of_degree;
	
	int c;
	int buffer[8];
	int buffer_counter = 0;
	int *char_buff = (int *)malloc(block_size * sizeof(int));
	int *deg_buff = (int *)malloc(numb_of_degree * sizeof(int));

	FILE *src = fopen(srcFlName, "r");
	FILE *deham_out = fopen("/Users/mac/Desktop/univer/proggin/oib/lab 10/deciphed_text.txt", "w");

	while (!feof(src))
	{
		//preparin -1 is place for DEGREES
		for(int i = 0; i < block_size; i++)
			char_buff[i] = 0;
		for(int i = 0; i < numb_of_degree; i++)
			char_buff[binDeg[i] - 1] = -1;
		//getting chars from binfile
		i = 0;
		char chr;
		while(i < block_size && (chr = fgetc(src)) != EOF)
		{
			if(i < block_size)
				char_buff[i] = chr - '0';
			i++;
		}
		
		//if not divided by 8 (13 numb for ex)
		if(i != block_size)
		{
			int t = 0;
			while (i > binDeg[t])
				t++;
			numb_of_degree = t;
			block_size = i;
		}
		//counting CONTROL bits 
		for(int cnt = 0; cnt < numb_of_degree; cnt++)
		{
			int step = binDeg[cnt];
			int start = binDeg[cnt] - 1;
			int summ = 0;
			int j = start;
			while(j < block_size)
			{
				int t = j;
				if(j == start)
					t++;
				for(; t < block_size && t < j + step; t++)
					summ += char_buff[t];
				j += step + step;
			}
			deg_buff[cnt] = summ % 2;
		}
		//trying to fix bad bits
		int bit_sum = -1;
		for(int j = 0; j < numb_of_degree; j++)
		{
			if(deg_buff[j] != char_buff[binDeg[j] - 1])
				bit_sum += binDeg[j];
			char_buff[bit_sum] = (char_buff[bit_sum] + 1) % 2;
		}
		//preparing for output
		i = 0;
		for(int j = 0; j < numb_of_degree; j++)
			char_buff[binDeg[j] - 1] = -1;
		while(i < block_size)
		{
			if(buffer_counter == 8)
				buffer_counter = 0;
			while(buffer_counter < 8 && i < block_size)
			{
				if(char_buff[i] != -1)
					buffer[buffer_counter++] = char_buff[i];
				i++;
			}
			if(buffer_counter == 8)
			{
				c = binToDec(buffer, 8);
				fputc(c, deham_out);
			}
		}
		
	}
	printf("1");
	fclose(src);
	fclose(deham_out);
	free(char_buff);
	free(deg_buff);
	return;
}