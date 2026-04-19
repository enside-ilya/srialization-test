#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdexcept>

struct ListNode{
    ListNode *prev=nullptr;
    ListNode *next=nullptr;
    ListNode *rand=nullptr;
    std::string data;
};


void serialization(std::vector<ListNode*>& vec_data,std::vector<int>&rand_indx){
    std::ofstream file("outlet.out",std::ios::binary);

    for(size_t i=0;i<vec_data.size();i++)
    {
        int len=vec_data[i]->data.length();
        file.write((const char*)&len,sizeof(len));

        if (len>0){
            file.write(vec_data[i]->data.c_str(),len);
        }
        int r=-1;
        if (rand_indx[i]>=0&&rand_indx[i]<vec_data.size()){
            r=rand_indx[i];
        }
        file.write((const char*)&r,sizeof(r));

    }
    file.close();
    if (file.fail()){
        throw std::runtime_error("error writing to file: outlet.out");
    }
}

void deleteList(ListNode* head) {
    while (head) {
        ListNode* tmp = head;
        head = head->next;
        delete tmp;
    }
}


int main()
{
    std::vector<ListNode*> vec_data;
    std::vector<int>rand_indx;

    std::ifstream file("inlet.in");
    if (!file.is_open()) {
        std::cerr << "Cannot open inlet.in\n";
        return 1;
    }

    std::string line;
    while(std::getline(file,line))
    {
        auto sep= line.rfind(";");
        if (sep==std::string::npos){
            continue;
        }
        ListNode* node= new ListNode();
        std::string data=line.substr(0,sep);
        node->data=data;
        vec_data.push_back(node);
        std::string rand_ix=line.substr(sep+1);
        //адаптация для винды, если файл был с линукса
        size_t end = rand_ix.find_last_not_of(" \t\r\n");
        if (end != std::string::npos) rand_ix.erase(end + 1);
        rand_indx.push_back(std::stoi(rand_ix));
    }

    if (vec_data.empty()) return 0;

    for (size_t i = 0; i < vec_data.size();i++) {
        if (i > 0) {
            vec_data[i]->prev = vec_data[i - 1];
        }
        if (i < vec_data.size() - 1){
            vec_data[i]->next = vec_data[i + 1];
        }
    }
    for(size_t i=0;i<vec_data.size();i++){
        int r=rand_indx[i];
        if (r>=0 && r<vec_data.size())
        {
            vec_data[i]->rand=vec_data[r];
        }
    }
    serialization(vec_data,rand_indx);

    deleteList(vec_data[0]);

}
