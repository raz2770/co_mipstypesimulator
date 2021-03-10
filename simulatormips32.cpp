#include<bits/stdc++.h>
using namespace std;
class registers
{
    public:
    string reg_str[32]={ "r0", "at", "v0", "v1", "a0", "a1", "a2", "a3","t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7","s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7","t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra"};
    map<string, int> reg_map;
    int *regs = new int[32]; 
   
    
};
class screen
{  
    public:
    void print_reg(int *regs, vector<pair<string, int>> &reg_vector) {
            for (int i = 0; i < 32; ++i) {
                cout << "$" <<  reg_vector[i].first << "[" << setw(2) << reg_vector[i].second << "] : " << regs[reg_vector[i].second] << endl;
            }
        }
    void print_userdata(map<int,int>&data,int b,int e)
    {
        for(int i=b;i<e;i+=4)
        {
            cout << "<Memory> " << "0x" << setfill('0') << setw(8) << hex << i << " : " << dec << data[i] << endl;
        }
    }
    void print_data(map<int, int> &data, int bgp, int bst,int egp,int est) {
         cout << endl << "       GLOBAL DATA       " << "          |       "<<"     STACK DATA      "<<endl;
        for (int i = bgp, j=bst; i < egp,j<est; i+=4,j+=4) {
            cout << "<Memory> " << "0x" << setfill('0') << setw(8) << hex << i << " : " << dec << data[i] <<"            |          "<< "<Memory> " << "0x" << setfill('0') << setw(8) << hex << j << " : " << dec << data[j]<<endl;
        }
    }
    
};
class parser
{   
    public:

    void parse(string file, vector<string> & code,map<string, int> &branches,map<int, int> &data,map<string, int> &data_r,int &data_p)

    {   
        ifstream infile(file); 
        if (!infile.is_open()) {
            cerr << "Error : File does not exists" << endl;
        
        }
        regex mode_e("\\s*\\.(\\w+)\\s*(\\w+)?\\s*(#.*)?");
        regex empty_e("\\s*(#.*)?");
        regex branch_e("\\s*(\\w+)\\s*:.*");
        regex br_empty_e("\\s*(\\w+)\\s*:\\s*(#.*)?");
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
                    break;
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
                        
                    }
                }
            }
            else if (mode == 1) { 
                if (regex_match(line, sm, branch_e)) {
                    if(branches[sm[1]] == 0) branches[sm[1]] = line_no;
                    else {
                        
                         cout << "Error at line " << line_no+1 << " : " << "invalid instruction"<< endl;
                       
                    }
                }
                code[line_no] = line;
            }
            else {
                
                 cout << "Error at line " << line_no+1 << " : " << "invalid instruction"<< endl;
                
            }
        }

    }
   
};
class execution
{   
    public:
    int get_reg(string s, map<string, int> &reg_map) {
        regex d_e("\\d+");
        smatch sm;
        if (regex_match(s, sm, d_e)) 
        {
            int r = stoi(s);
            if (r == 1 || r == 26 || r == 27) return -1;
            return stoi(s);
        }
        if (s == "zero" || s == "r0") return 0;
        if (reg_map[s] != 0 && reg_map[s] != 1 && reg_map[s] != 26 && reg_map[s] != 27) 
         return reg_map[s];
        return -1;
    }
    void execute(vector<string> & code, map<string, int> &reg_map,int *regs,map<string, int> &data_r,map<int, int> &data,map<string, int> &branches)
    {   
       
        regex br_empty_e("\\s*(\\w+)\\s*:\\s*(#.*)?"); 
        
        regex r_format_e("\\s*(\\w+\\s*:)?\\s*(\\w+)\\s+\\$(\\w+)\\s*,\\s*\\$(\\w+)\\s*,\\s*\\$(\\w+)\\s*(#.*)?");
        
        regex i_format_e("\\s*(\\w+\\s*:)?\\s*(\\w+)\\s+\\$(\\w+)\\s*,\\s*\\$(\\w+)\\s*,\\s*(\\-?\\d+)\\s*(#.*)?");
        
        regex load_e("\\s*(\\w+\\s*:)?\\s*([a-z][a-z])\\s+\\$(\\w+)\\s*,\\s*(\\-?\\d+)\\s*\\(\\s*\\$(\\w+)\\s*\\)\\s*(#.*)?");
        
        regex la_e("\\s*(\\w+\\s*:)?\\s*la\\s+\\$(\\w+)\\s*,\\s*(\\w+)\\s*(#.*)?");
        regex li_e("\\s*(\\w+\\s*:)?\\s*li\\s+\\$(\\w+)\\s*,\\s*(\\w+)\\s*(#.*)?");
        regex lw_e("\\s*(\\w+\\s*:)?\\s*lw\\s+\\$(\\w+)\\s*,\\s*(\\w+)\\s*(#.*)?");
        regex sw_e("\\s*(\\w+\\s*:)?\\s*sw\\s+\\$(\\w+)\\s*,\\s*(\\w+)\\s*(#.*)?");
        
        regex cond_branch_e("\\s*(\\w+\\s*:)?\\s*(\\w+)\\s+\\$(\\w+)\\s*,\\s*\\$(\\w+)\\s*,\\s*(\\w+)\\s*(#.*)?");
        
        regex jump_e("\\s*(\\w+\\s*:)?\\s*j\\s+(\\w+)\\s*(#.*)?");
        
        regex syscall_e("\\s*(\\w+\\s*:)?\\s*syscall\\s*(#.*)?");
        
       
        smatch sm;
       for (int i = 0; i < code.size(); ++i) { 
        if (code[i] == "") continue; 
        else if (regex_match(code[i], sm, br_empty_e)) continue;
        else if (regex_match(code[i], sm, r_format_e)) { 
            int reg_d = get_reg(sm[3], reg_map);
            int reg_s = get_reg(sm[4], reg_map);
            int reg_t = get_reg(sm[5], reg_map);
            if (reg_d == -1 || reg_s == -1 || reg_t == -1) {
                cout << "Error at line " << i+1 << " : " <<"invalid instruction R"<< endl;
                break;
            }
            if (sm[2] == "add") 
            {
                regs[reg_d] = regs[reg_s] + regs[reg_t];
            }
            else if (sm[2] == "sub") {
                regs[reg_d] = regs[reg_s] - regs[reg_t];
            }
            else if (sm[2] == "mul") {
                regs[reg_d] = regs[reg_s] * regs[reg_t];
            }
            
            else if (sm[2] == "slt") {
                regs[reg_d] = regs[reg_s] < regs[reg_t];
            }
            else {
                cout << "Error at line " << i+1 << " : " <<"invalid instruction R ADDRESS"<< endl;
                break;
            }
            regs[0] = 0;
        }
        else if (regex_match(code[i], sm, i_format_e)) { 
            int reg_d = get_reg(sm[3], reg_map);
            int reg_s = get_reg(sm[4], reg_map);
            int imm = stoi(sm[5]);
            if (reg_d == -1 || reg_s == -1) {
                cout << "Error at line " << i+1 << " : " <<"invalid instruction I"<< endl;
                break;
            }
            if (sm[2] == "addi") {
              
                regs[reg_d] = regs[reg_s] + imm;
            }
            
           
            else {
                cout << "Error at line " << i+1 << " : " <<"invalid instruction I ADDRESS"<< endl;
                break;
            }
        }
       
        else if (regex_match(code[i], sm, la_e)) { 
            int reg_d = get_reg(sm[2], reg_map);
            string dt = sm[3];
            int adr = data_r[dt];
            if (adr == 0 || reg_d == -1) {
                cout << "Error at line " << i+1 << " : " <<"invalid instruction LA $R "<< endl;
                break;
            }
            regs[reg_d] = adr;
        }
        else if (regex_match(code[i], sm, lw_e)) { 
            int reg_d = get_reg(sm[2], reg_map);
            string dt = sm[3];
            int adr = data_r[dt];
            if (adr == 0 || reg_d == -1) {
                cout << "Error at line " << i+1 << " : " <<"invalid instruction LW $R"<< endl;
                break;
            }
            regs[reg_d] = data[adr];
        }
        else if (regex_match(code[i], sm, sw_e)) { 
            int reg_d = get_reg(sm[2], reg_map);
            string dt = sm[3];
            int adr = data_r[dt];
            if (adr == 0 || reg_d == -1) {
                 cout << "Error at line " << i+1 << " : " <<"invalid instruction SW $r"<< endl;
                break;
            }
            data[adr] = regs[reg_d];
        }
         else if (regex_match(code[i], sm, li_e))
        {
            int reg_d = get_reg(sm[2], reg_map);
            int imm = stoi(sm[3]); 
            if ( reg_d == -1) {
                 cout << "Error at line " << i+1 << " : " <<"invalid instruction SW $r"<< endl;
                break;
            } 
            regs[reg_d]=imm;
        }
        else if (regex_match(code[i], sm, load_e)) {
            int reg_d = get_reg(sm[3], reg_map);
            int reg_s = get_reg(sm[5], reg_map);
            int imm = stoi(sm[4]);
            if (reg_d == -1 || reg_s == -1 || imm%4 != 0) {
                cout << "Error at line " << i+1 << " : " <<"invalid instruction SW ,LW"<< endl;
                break;
            }
            if (sm[2] == "lw") {
                regs[reg_d] = data[imm+regs[reg_s]];
            }
            else if (sm[2] == "sw") {
                data[imm+regs[reg_s]] = regs[reg_d];
            }
            else {
               cout << "Error at line " << i+1 << " : " <<"invalid instruction LW,SW ADDRESS"<< endl;
               break;
            }
        }
        else if (regex_match(code[i], sm, cond_branch_e)) { 
            int reg_1 = get_reg(sm[3], reg_map);
            int reg_2 = get_reg(sm[4], reg_map);
            int br = branches[sm[5]];
            if (reg_1 == -1 || reg_2 == -1) {
                cout << "Error at line " << i+1 << " : " <<"invalid instruction beq"<< endl;
                break;
            }
            if (br == 0) {
                cout << "Error at line " << i+1 << " : " <<"invalid instruction bne"<< endl;
                break;
            }
            bool eq = (regs[reg_1] == regs[reg_2]);
           
            if (sm[2] == "bne") {
                if (!eq) {
                    i = br-1;
                }
            }
            else {
                cout << "Error at line " << i+1 << " : " <<"invalid instruction"<< endl;
                break;
            }
        }
        else if (regex_match(code[i], sm, jump_e)) { // j
            int br = branches[sm[2]];
            if (br == 0) {
                 cout << "Error at line " << i+1 << " : " <<"invalid instruction"<< endl;
                 break;
            }
            i = br-1;
        }
        else if (regex_match(code[i], sm, syscall_e)) { // halt
             int reg_d = get_reg("v0", reg_map);
             if(regs[reg_d]==10)
             break;
             else
             continue;

        }
       
        else {
            
            
            
            cout << "Error at line " << i+1 << " : " <<"invalid instruction end"<< endl;
            break;
        }

        
        
    }
    }
   
};

class mipssimulator:screen ,registers,parser,execution
{   
    public:
    string input_file;
   
    mipssimulator(string input_file)
    {
          this->input_file=input_file;
        
    }
    int run()
    {
    string file=input_file; 
    int mode_exec = 0; 
    for (int j = 0; j < 32; ++j) {
        reg_map[reg_str[j]] = j;
    }

    vector<pair<string, int>> reg_vector; 
    for (pair<string, int> i:reg_map) {
        reg_vector.push_back(i);
    }
    
    
    sort(reg_vector.begin(), reg_vector.end(), [](const pair<string, int> &left, const pair<string, int> &right) {
        return left.second < right.second;
    });


    cout << "MIPS Simulator" << endl << endl;

    
    vector<string> code; 
   
    for (int k = 0; k < 32; ++k) { 
        regs[k] = 0;
    }

    map<int, int> data; 
    map<string, int> data_r;
    
    int data_p_st = 10000000; 
    int data_p = data_p_st;
    int global_p = 20000000;
    int stack_p = 90000000;
    regs[get_reg("gp", reg_map)] = global_p;
    regs[get_reg("sp", reg_map)] = stack_p;

    

    map<string, int> branches;
    
   
   

    parse(file, code, branches,data,data_r,data_p);
    execute(code,reg_map,regs,data_r,data,branches);
    
    print_reg(regs, reg_vector); 
    cout << endl << "USER DATA" << endl;
    print_userdata(data, data_p_st, data_p);
    print_data(data, global_p,stack_p-200, global_p+200,stack_p);
   
    return 0;
    }
  
    
    
};
int main() {
    string file;
    cin>>file;
    mipssimulator *mp=new mipssimulator(file);
    mp->run();
}
