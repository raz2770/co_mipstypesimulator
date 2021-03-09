#include<bits/stdc++.h>
using namespace std;
class parser
{   
    public:
    int parse(string file, vector<string> & code,map<string, int> &branches,map<int, int> &data,map<string, int> &data_r,int &data_p)

    {   
        ifstream infile(file); 
        if (!infile.is_open()) { 
            cerr << "Error : File does not exists" << endl;
            return 0;
        }
         regex mode_e("\\s*\\.(\\w+)\\s*(\\w+)?\\s*(#.*)?"); 
        regex empty_e("\\s*(#.*)?"); 
        regex branch_e("\\s*(\\w+)\\s*:.*"); 
        regex br_empty_e("\\s*(\\w+)\\s*:\\s*(#.*)?"); 
        regex r_format_e("\\s*(\\w+\\s*:)?\\s*(\\w+)\\s+\\$(\\w+)\\s*,\\s*\\$(\\w+)\\s*,\\s*\\$(\\w+)\\s*(#.*)?");
        regex i_format_e("\\s*(\\w+\\s*:)?\\s*(\\w+)\\s+\\$(\\w+)\\s*,\\s*\\$(\\w+)\\s*,\\s*(\\-?\\d+)\\s*(#.*)?");
        regex load_e("\\s*(\\w+\\s*:)?\\s*([a-z][a-z])\\s+\\$(\\w+)\\s*,\\s*(\\-?\\d+)\\s*\\(\\s*\\$(\\w+)\\s*\\)\\s*(#.*)?");
        regex la_e("\\s*(\\w+\\s*:)?\\s*la\\s+\\$(\\w+)\\s*,\\s*(\\w+)\\s*(#.*)?");
        regex lw_e("\\s*(\\w+\\s*:)?\\s*lw\\s+\\$(\\w+)\\s*,\\s*(\\w+)\\s*(#.*)?");
        regex sw_e("\\s*(\\w+\\s*:)?\\s*sw\\s+\\$(\\w+)\\s*,\\s*(\\w+)\\s*(#.*)?");
        regex cond_branch_e("\\s*(\\w+\\s*:)?\\s*(\\w+)\\s+\\$(\\w+)\\s*,\\s*\\$(\\w+)\\s*,\\s*(\\w+)\\s*(#.*)?");
        regex jump_e("\\s*(\\w+\\s*:)?\\s*j\\s+(\\w+)\\s*(#.*)?");
        regex halt_e("\\s*(\\w+\\s*:)?\\s*halt\\s*(#.*)?");
        regex data_word_e("\\s*(\\w+)\\s*:\\s*\\.(\\w+)\\s+([\\-\\w\"'\\s,]+)\\s*(#.*)?");
        smatch sm;
        int line_no=-1;
        int mode = -1;
        string line;

        while (getline(infile, line)) 
        { 
            line_no++;
            code.push_back("");
            if (regex_match(line, sm, empty_e)) 
            continue;
            if(regex_match(line, sm, mode_e)) 
            { 
                if (sm[1] == "data" && sm[2] == "") 
                mode = 0; 
                else if (sm[1] == "text" && sm[2] == "") 
                mode = 1;
                else if (sm[1] == "globl" && sm[2] == "main") 
                {

                }
                else { 
                    cout << "Error at line " << line_no+1 << " : " << "invalid instruction"<< endl;
                    return 0;
                }
                continue;
            }
            if (mode == 0) {
                if (regex_match(line, sm, data_word_e)) {
                    if (sm[2] == "word") {
                        string els = sm[3];
                        cout<<"els"<<els<<endl;
                        

                        els = regex_replace(els, regex("\\s+$"), "");
                        els = regex_replace(els, regex(",\\s*"), "|");
                        vector<string> v;
                    
                        v.clear();
                        stringstream ss(els);
                        string val;

                        while(getline(ss, val, '|')) {
                            if (val.size() != 0) 
                            v.push_back(val);
                        }
                        data_r[sm[1]] = data_p;
                        for (string s : v) {
                            int t = stoi(s);
        
                            data[data_p] = t;
                            data_p += 4;
                        }
                    }
                    
                    else {
                        
                         cout << "Error at line " << line_no+1 << " : " << "invalid instruction"<< endl;
                        return 0;
                    }
                }
            }
            else if (mode == 1) { 
                if (regex_match(line, sm, branch_e)) {
                    if(branches[sm[1]] == 0) branches[sm[1]] = line_no;
                    else {
    
                         cout << "Error at line " << line_no+1 << " : " << "invalid instruction"<< endl;
                        return 0;
                    }
                }
                code[line_no] = line;
            }
            else {
               
                 cout << "Error at line " << line_no+1 << " : " << "invalid instruction"<< endl;
                return 0;
            }
        }

    }

};

class screen
{

};
class memory
{

};
class mipssimulator
{

};
int main()
{
    return 0;
}
