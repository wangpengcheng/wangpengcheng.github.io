/* 题目描述:

https://leetcode-cn.com/problems/max-increase-to-keep-city-skyline/

在二维数组grid中，grid[i][j]代表位于某处的建筑物的高度。
我们被允许增加任何数量（不同建筑物的数量可能不同）的建筑物的高度。 高度 0 也被认为是建筑物。
最后，从新数组的所有四个方向（即顶部，底部，左侧和右侧）观看的“天际线”必须与原始数组的天际线相同。 
城市的天际线是从远处观看时，由所有建筑物形成的矩形的外部轮廓。 
请看下面的例子。
建筑物高度可以增加的最大总和是多少？

例子：
输入： grid = [[3,0,8,4],[2,4,5,7],[9,2,6,3],[0,3,1,0]]
输出： 35
解释： 
The grid is:
[ [3, 0, 8, 4], 
  [2, 4, 5, 7],
  [9, 2, 6, 3],
  [0, 3, 1, 0] ]
从数组竖直方向（即顶部，底部）看“天际线”是：[9, 4, 8, 7]
从水平水平方向（即左侧，右侧）看“天际线”是：[8, 7, 9, 3]
在不影响天际线的情况下对建筑物进行增高后，新数组如下：
gridNew = [ [8, 4, 8, 7],
            [7, 4, 7, 7],
            [9, 4, 8, 7],
            [3, 3, 3, 3] ]


说明:

    1 < grid.length = grid[0].length <= 50。
    grid[i][j] 的高度范围是： [0, 100]。
    一座建筑物占据一个grid[i][j]：换言之，它们是 1 x 1 x grid[i][j] 的长方体。


*/
/*
主要思路：找到对应行列最大值;遍历每个数取行列的最小值，空间复杂度为O(2*grid.length),时间复杂度为O(2*grid.length^2)

		解决思路;
		1.遍历查找列最大值;
		2.将列中数据都改为最大值
		2.遍历行查找行最大值;
		3.将行中数据都改为最大值
		
*/

#include <iostream>
#include <vector>


using namespace std;

void print_grid(vector<vector<int>>& input_grid)
{
		cout<<"[ \n";
	for(auto temp : input_grid){
		cout<<"[";
		for(auto temp2 : temp){
			cout<< temp2<<",";
		}
		cout<<"], \n";
	}
	cout<<"] \n"<<endl;
}


//main function
int maxIncreaseKeepingSkyline(vector<vector<int>>& grid) {
		int result=0;
		unsigned int col,row;
		col=row=grid.size();
		unsigned int col_max[col],row_max[row];
		//遍历行
		for(unsigned int col_i=0;col_i<col;++col_i)
		{
			int c_max=0;
			for(unsigned int row_i=0;row_i<col;++row_i)
			{
				if(grid[col_i][row_i]>c_max){
					c_max=grid[col_i][row_i];
				};
			}
			col_max[col_i]=c_max;

		}
		for(unsigned int row_i=0;row_i<col;++row_i)
		{
			int r_max=0;
			for(unsigned int col_i=0;col_i<row;++col_i)
			{
				if(grid[col_i][row_i]>r_max){
					r_max=grid[col_i][row_i];
				};
			}
			row_max[row_i]=r_max;

		}
		for(unsigned int x=0;x<col;++x)
		{
			for(unsigned int y=0;y<row;++y)
			{
				int temp=(row_max[y]<col_max[x])?row_max[y]:col_max[x];
				result+=temp-grid[x][y];
			}

		}
        return result;
    };

int main(int argc, char const *argv[]) {
	//test input
	vector< vector<int> > input_grid={{3, 0, 8, 4},{2, 4, 5, 7},{9, 2, 6, 3},{0, 3, 1, 0}};
	//output string
	print_grid(input_grid);
	//my result number
	auto  my_result=maxIncreaseKeepingSkyline(input_grid);
	//print_vector(my_result);
	std::cout<<"result"<<my_result<<std::endl;
	print_grid(input_grid);
	//print_vector(my_result);
	return 0;
}
/*
//优质解答1：
//思路一样，在第一次遍历时找出两种最大值

int maxIncreaseKeepingSkyline(vector<vector<int>>& grid) {
        int high1[50] = {0};
        int high2[50] = {0};
        int ret = 0, tmp;
        for (int i = 0; i < grid.size(); i++)
        {
            for (int j = 0; j < grid[i].size(); j++)
            {
                if (grid[i][j] > high1[i])
                {
                    high1[i] = grid[i][j];
                }
                if (grid[i][j] > high2[j])
                {
                    high2[j] = grid[i][j];
                }
            }
        }
        for (int i = 0; i < grid.size(); i++)
        {
            for (int j = 0; j < grid[i].size(); j++)
            {
                if (high1[i] < high2[j])
                {
                    tmp = high1[i];
                }
                else
                {
                    tmp = high2[j];
                }
                ret = ret + tmp - grid[i][j];
            }
        }
        return ret;
    }

    优质解答二：
    思路:和上面相同使用三目运算符优化速度。



static const auto __ = []{ std::ios::sync_with_stdio(false); std::cin.tie(nullptr); return 0; }();

class Solution {
public:
    int maxIncreaseKeepingSkyline(vector<vector<int>>& grid) {
//        int lenX = grid.size();
//        int lenY = grid[0].size();
        int len = grid.size();
        int topSkyline[len] = {0};
        int leftSkyline[len] = {0};

        for(int i = 0; i < len; ++i)
        {
            for(int j = 0; j < len; ++j)
            {
                //获取左侧天际线
               //if(leftSkyline[i] < grid[i][j])
               //    leftSkyline[i] = grid[i][j];
                //获取上侧天际线
               //if(topSkyline[i] < grid[j][i])
               //     topSkyline[i] = grid[j][i];
               leftSkyline[i] = leftSkyline[i] < grid[i][j] ? grid[i][j] : leftSkyline[i]; 
               topSkyline[i] = topSkyline[i] < grid[j][i] ? grid[j][i]: topSkyline[i]; 
            }
        }
        int IncreaseSum = 0;
        for(int i = 0; i < len; ++i)
        {
            for(int j = 0; j < len; ++j)
            {
                if( (grid[i][j] == topSkyline[j]) || (grid[i][j] == leftSkyline[i]))
                {
                    continue;
                }
                //if(leftSkyline[i] > topSkyline[j])
                //{
                //    IncreaseSum += topSkyline[j]-grid[i][j];
                //}
                //else
                //{
                //    IncreaseSum += leftSkyline[i]-grid[i][j];
                //}
                IncreaseSum += ((leftSkyline[i] < topSkyline[j]) ? leftSkyline[i] : topSkyline[j]) -grid[i][j];
            }
        }
        return IncreaseSum;
    }
};

*/