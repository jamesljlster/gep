#include <iostream>
#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include <vector>

#include <gep.hpp>

using namespace std;

namespace gep
{
	vector<string> gep_chro_split(const char* chroStr)
	{
		vector<string> ret;
		string tmp;

		int len = strlen(chroStr);
		for(int i = 0; i < len; i++)
		{
			switch(chroStr[i])
			{
				case '"':
				case ' ':
					break;

				case ',':
					ret.push_back(tmp);
					tmp.clear();
					break;

				default:
					tmp.push_back(chroStr[i]);
					break;
			}
		}
		
		if(!tmp.empty())
		{
			ret.push_back(tmp);
		}

		return ret;
	}

	union GEP_NODE gep_parse_node(const char* nodeStr)
	{
		union GEP_NODE tmpNode;

		// Try to parse string as real number
		char* tmpPtr;
		double tmpValue = strtod(nodeStr, &tmpPtr);
		if(tmpPtr == nodeStr)
		{
			// Try to find "var"
			if(strncmp(nodeStr, "var", 3) == 0)
			{
				int tmpIndex = strtol(nodeStr + 3, &tmpPtr, 10);
				if(tmpPtr == nodeStr + 3)
				{
					std::string tmp = __FUNCTION__;
					tmp += "(): Unexpected GEP chromosome format!";
					throw std::invalid_argument(tmp);
				}
				else
				{
					tmpNode.type = GEP_TYPE_VARIABLE;
					tmpNode.var.index = tmpIndex;
				}
			}
			else
			{
				// Parse as operator
				int tmpOp = -1;
				for(int i = 0; i < GEP_OP_AMOUNT; i++)
				{
					if(strcmp(gep_func_box[i].name, nodeStr) == 0)
					{
						tmpOp = i;
						break;
					}
				}

				if(tmpOp < 0)
				{
					std::string tmp = __FUNCTION__;
					tmp += "(): Unexpected GEP chromosome format!";
					throw std::invalid_argument(tmp);
				}
				else
				{
					tmpNode.type = GEP_TYPE_OPERATOR;
					tmpNode.op.op = tmpOp;
				}
			}
		}
		else
		{
			// Parse as terminal
			tmpNode.type = GEP_TYPE_TERMINAL;
			tmpNode.t.value = tmpValue;
		}

		return tmpNode;
	}

	vector<union GEP_NODE> gep_parse(const char* chroStr)
	{
		vector<union GEP_NODE> ret;

		vector<string> nodeList = gep_chro_split(chroStr);
		for(int i = 0; i < nodeList.size(); i++)
		{
			ret.push_back(gep_parse_node(nodeList[i].c_str()));
		}

		return ret;
	}

}
