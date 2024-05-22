/* 题目描述:

https://leetcode-cn.com/problems/remove-outermost-parentheses/

有效括号字符串为空 ("")、"(" + A + ")" 或 A + B，其中 A 和 B 都是有效的括号字符串，
+ 代表字符串的连接。例如，""，"()"，"(())()" 和 "(()(()))" 都是有效的括号字符串。
如果有效字符串 S 非空，且不存在将其拆分为 S = A+B 的方法，我们称其为原语（primitive），
其中 A 和 B 都是非空有效括号字符串。
给出一个非空有效字符串 S，考虑将其进行原语化分解，
使得：S = P_1 + P_2 + ... + P_k，其中 P_i 是有效括号字符串原语。
对 S 进行原语化分解，删除分解中每个原语字符串的最外层括号，返回 S 。
展示：

示例 1：

输入："(()())(())"
输出："()()()"
解释：
输入字符串为 "(()())(())"，原语化分解得到 "(()())" + "(())"，
删除每个部分中的最外层括号后得到 "()()" + "()" = "()()()"。

示例 2：

输入："(()())(())(()(()))"
输出："()()()()(())"
解释：
输入字符串为 "(()())(())(()(()))"，原语化分解得到 "(()())" + "(())" + "(()(()))"，
删除每隔部分中的最外层括号后得到 "()()" + "()" + "()(())" = "()()()()(())"。

示例 3：

输入："()()"
输出：""
解释：
输入字符串为 "()()"，原语化分解得到 "()" + "()"，
删除每个部分中的最外层括号后得到 "" + "" = ""。


*/
/*
主要思路：对字符串遍历设置需要匹配“)”个数，为0则检查是否为原语，是删除最外层括号不是则继续遍历子串

		解决思路;
		1.遍历看到“(”;need+1,记录start;
		2.看到“)”,need-1;
		3.need=0,记录end;
		4.添加start与end之间的字符
		
*/

#include <iostream>
#include <vector>


using namespace std;

void print_vector(std::vector<int> v){
	for(auto i : v) {		
		cout<<i<<endl;
	}
};
//main function
string removeOuterParentheses(string S) {
		string result="";
		unsigned int need=0,start=0,end=0;
        for (unsigned int i = 0; i < S.size(); ++i)
        {
        	if(S[i]=='(')
        	{
        		// if(end==0){
        		// 	start=i;
        		// }
        		++need;
        	}else if(S[i]==')')
        	{
        		--need;
        	};
        	//获取尾部位置
        	if(need==0){
        		end=i;
        		// printf("i: %d\t", i);
        		// printf("start: %d\t", start);
        		// printf("end: %d\n", end);
        		//截取字符串
        		if(start!=end&&end>start){
        			// cout<<S.substr(start+1,end-start-1)<<endl;
        			result+=S.substr(start+1,end-start-1);
        		}
        		start=end+1;
        	}
        }
        return result;
    };

int main(int argc, char const *argv[]) {
	//input string
	std::string input_string1="(()())(())(()(()))";
	std::string input_string2="(()())(())";
	std::string input_string3="()()";
	//output string
	std::string output="";
	//my result number
	auto  my_result=removeOuterParentheses(input_string3);
	//print_vector(my_result);
	std::cout<<"result"<<my_result<<std::endl;

	//print_vector(my_result);
	return 0;
}
/*
//优质解答：
//思路一样，分开统计

  string res;
		int leftCount = 0, rightCount = 0;
		int startIndex = 0;
		for (size_t i = 0; i < S.length(); i++)
		{
			if (S[i] == '(')
			{
				if (rightCount == 0)
					startIndex = i;
				rightCount++;
			}
			else if (S[i] == ')')
			{
				leftCount++;
				if (leftCount == rightCount)
				{
					res += S.substr(startIndex + 1, i - startIndex - 1);
					leftCount = 0;
					rightCount = 0;
				}
			}
		}
		return res;
    }
*/