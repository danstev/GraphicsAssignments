/*
Daniel Ward

For height mapping, but used for perlin noise, future could include passing a int and creating a square heightmap.

*/
#include <glm/glm.hpp>
#include <iostream>

using namespace std;

class HeightMapping {

public:

	float heightMap[51][51];
	float bottom = 0.5f, top = 0.5f;

	void printMap()
	{
		cout << "Printing Map: " << endl;
		for (int x = 0; x < 51; x++)
		{
			for (int y = 0; y < 51; y++)
			{
				cout << heightMap[x][y] << ' ';
			}
			cout << endl;
		}
		cout << endl;
	}

	void setAllZero()
	{
		for (int x = 0; x < 51; x++)
		{
			for (int y = 0; y < 51; y++)
			{
				heightMap[x][y] = 0.0f;
			}
		}
	}

	void randomlyAddOnes()
	{
		for (int x = 0; x < 51; x++)
		{
			for (int y = 0; y < 51; y++)
			{
				if (rand() % 100 > 98)
				{
					heightMap[x][y] = 1.00f;
				}
			}
		}
	}

	void floorEverything()
	{
		for (int x = 0; x < 51; x++)
		{
			for (int y = 0; y < 51; y++)
			{
				if (heightMap[x][y] < 0)
				{
					heightMap[x][y] = 0.00f;
				}
			}
		}
	}

	void smooth()
	{
		for (int x = 1; x < 50; x++)
		{
			for (int y = 1; y < 50; y++)
			{
				heightMap[x][y] = (heightMap[x][y] + heightMap[x + 1][y] + heightMap[x - 1][y] + heightMap[x][y + 1] + heightMap[x][y - 1]) / 5;
			}
		}
	}

	void setBottomTop()
	{
		for (int x = 0; x < 51; x++)
		{
			for (int y = 0; y < 51; y++)
			{
				if (heightMap[x][y] < bottom)
					bottom = heightMap[x][y];

				if (heightMap[x][y] > top)
					top = heightMap[x][y];
			}
		}
	}

	HeightMapping()
	{
		float heightMap2[51][51];
		setAllZero();
		randomlyAddOnes();

		for (int x = 0; x < 51; x++)
		{
			for (int y = 0; y < 51; y++)
			{
				if (heightMap[x][y] == 0.0f)
				{
					int xposition = 0; //didnt like xpos as xpos and didnt like it not being with the other ints.
					int xneg = 0, ypos = 0, yneg = 0, xfin, yfin;

					int count = 0;
					while (xposition == 0)
					{
						if (x + count == 51)
						{
							xposition = count;
						}
						if (heightMap[x + count][y] == 1)
						{
							xposition = count;
						}
						count++;
					}
					count = 0;

					while (xneg == 0)
					{
						if (x - count == 0 || x == 0)
						{
							xneg = count;
						}
						if (heightMap[x - count][y] == 1)
						{
							xneg = count;
						}
						count++;
					}
					count = 0;

					while (ypos == 0)
					{
						if (y + count == 51)
						{
							ypos = count;
						}
						if (heightMap[x][y + count] == 1)
						{
							ypos = count;
						}
						count++;
					}
					count = 0;

					while (yneg == 0)
					{
						if (y - count == 0 || y == 0)
						{
							yneg = count;
						}
						if (heightMap[x][y - count] == 1)
						{
							yneg = count;
						}
						count++;
					}
					count = 0;

					if (xposition < xneg)
					{
						xfin = xposition;
					}
					else
					{
						xfin = xneg;
					}

					if (ypos < yneg)
					{
						yfin = ypos;
					}
					else
					{
						yfin = yneg;
					}

					float d = (float)(xfin + yfin) / 2;
					d = d / 10;
					//cout << d << endl;

					heightMap2[x][y] = d;
				}
			}
		}

		for (int x = 0; x < 51; x++)
		{
			for (int y = 0; y < 51; y++)
			{
				heightMap[x][y] = heightMap2[x][y];
			}
		}
		floorEverything();
		smooth();
		setBottomTop();
	}

	float getHeight(int x, int y)
	{
		return heightMap[x][y];
	}



	float getHeight(float x, float y)
	{
		float height, xdif, ydif;
		int xpos, ypos;
		xpos = (int)x; ypos = (int)y;
		xdif = x - xpos; ydif = y - ypos;
		
		height = (glm::mix(heightMap[xpos][ypos], heightMap[xpos + 1][ypos], xdif) + glm::mix(heightMap[xpos][ypos], heightMap[xpos][ypos + 1], ydif)) / 2;
		
		//cout << height << endl;
		return height;
	}
};