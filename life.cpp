#include <iostream>
#include <random>
#include <cstdlib>
#include <windows.h>
#include <time.h>

using namespace std;

struct cell {
	bool is_live : 1;
};


void start(cell **config, int width, int height) //set the starting config
{
	srand(time(NULL));
	for (int i = 0; i < width; i++) 
		for (int j = 0; j < height; j++) 
		{
			int num = rand() % 10000 + 1;
			if (num % 2 == 0)
				config[i][j].is_live = 1;
            else
				config[i][j].is_live = 0;
		}
}

void print_config(cell **config, int width, int height) // print current configuration on the screen
{
	for (int i = 0; i < width; i++) 
	{
		for (int j = 0; j < height; j++) 
		{
			if (config[i][j].is_live == 1) 
				cout << '*';
			else 
				cout << ' ';
			cout << ' ';
		}
		cout << endl;
	}
}

int count_live_cells(cell **config, int width, int height) //number of live cells in each config 
{
	int c = 0;
	for (int i = 0; i < width; i++) 
		for (int j = 0; j < height; j++) 
			if (config[i][j].is_live == 1) 
				c++;
    return c;
}

void neighbours(int neighb_arr[8][2], int x, int y) //the way to find neighbours of the cell with coordinates (x,y) on the field
{
	int k = 0;
	for (int i = x - 1; i <= x + 1; i++)
		for (int j = y - 1; j <= y + 1; j++)
		{
			if (i == x && j == y)
				continue;
			neighb_arr[k][0] = i;
			neighb_arr[k][1] = j;
			k++;
		}
}

int live_neighbours(cell **config, int x, int y, int width, int height) //count live neighbours of the cell with coordinates (x,y)
{
	int neighb_arr[8][2];
	int new_x, new_y, c = 0;
    neighbours(neighb_arr, x, y);
    for (int i = 0; i < 8; i++) 
	{
		new_x = neighb_arr[i][0];
		new_y = neighb_arr[i][1];

		if (new_x < 0 || new_y < 0)
			continue;
		if (new_x >= width || new_y >= height)
			continue;
        if (config[new_x][new_y].is_live == 1) 
			c++;
	}
    return c;
}

void next_gen(cell **config, cell **previous_config, int width, int height) //move to the next generation 
{
	int live_neighb;
	cell myCell;
    for (int i = 0; i < width; i++) 
		for (int j = 0; j < height; j++) 
		{
			myCell = previous_config[i][j];
			live_neighb = live_neighbours(previous_config, i, j, width, height);
            if (myCell.is_live == 0)
			{
				if (live_neighb == 3)
					config[i][j].is_live = 1;
			}
			else
			{
				if (live_neighb < 2 || live_neighb > 3)
				    config[i][j].is_live = 0;
			}
		}
}

int compare(cell** conf_1, cell** conf_2, int width, int height) //compare if two configs are equal
{
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
			if (conf_1[i][j].is_live != conf_2[i][j].is_live)
				return -1;
	return 0;
}

void copy_config(cell **old, cell **neu, int width, int height) //copy configuration
{
	for (int i = 0; i < width; i++) 
		for (int j = 0; j < height; j++)
		    neu[i][j] = old[i][j];
}


int main()
{
	int width, height;
	cout << "Input the width of the field, then press 'Enter':  "; cin >> height; // for correct definitions of width and height,
	cout << "Input the height of the field, then press 'Enter':  "; cin >> width; //  as it seems to a human
	cell** config = new cell * [width];
	for (int i = 0; i < width; i++)
		config[i] = new cell[height];
	cell **prev_config = new cell * [width];
	for (int i = 0; i < width; i++)
		prev_config[i] = new cell[height];

	start(config, width, height);
	int live_cells = 0;
	bool is_optimal = 0;

	while (1)
	{
		cout << endl;
		cout << endl;
		print_config(config, width, height);
		copy_config(config, prev_config, width, height);
		next_gen(config, prev_config, width, height);

		is_optimal = compare(config, prev_config, width, height) == 0;
		live_cells = count_live_cells(config, width, height);

		if (is_optimal == 1) 
		{
			cout << "This configuration is optimal. "<< count_live_cells(config, width, height) <<" cells survived." << endl;
			break;
		}

		if (live_cells == 0) 
		{
			cout << "All cells died." << endl;
			break;
		}
		Sleep(1500);
	}
    return 0;
}

