#include <bits/stdc++.h>
using namespace std;

int N = 3;
int maxx = 1e9+1;


typedef struct node{

    vector<int> keys;
    vector<node*> pointers;
    int isLeaf;

}Node;

Node* init_node(int is_leaf)
{
    Node* node = new Node;
    node->keys = vector<int>(N,maxx);
    node->pointers = vector<Node*>(N+1);
    node->isLeaf = is_leaf;
    return node;
}

Node* insert(Node* root,int value)
{
    // root is leaf
    if(root->isLeaf == 1)
    {
        if(root->keys[N-1] == maxx)
        {
            // means there is still space in this node
            int ind = 0;
            while(ind < N)
            {
                if (root->keys[ind] == maxx)
                {
                    root->keys[ind] = value;
                }
            }
        }
    }
}



int main(int argc,char const *argv[])
{

    // Individually store kar sakti hai
    string query_type[4] = {"FIND","INSERT","COUNT","RANGE"};
    string filename;

    if (argc <= 1)
    {
        cout << "Filename not given\n";
    }
    else
    {
        Node* root = init_node(1);
        filename = argv[1];
        ifstream file(filename);
        string line;
        int value1,value2;
        while(getline(file,line))
        {
            
            if(line.find(query_type[0]) != string::npos)
            {
                // FIND
                istringstream (line.substr(5)) >> value1;
                // cout << query_type[0] << " " << value1 << '\n';
            }
            else if(line.find(query_type[1]) != string::npos)
            {
                // INSERT
                istringstream (line.substr(7)) >> value1;
                // cout << query_type[1] << " " << value1 << '\n';
            }
            else if(line.find(query_type[2]) != string::npos)
            {
                // COUNT
                istringstream (line.substr(6)) >> value1;
                // cout << query_type[2] << " " << value1 << '\n';
            }
            else if(line.find(query_type[3]) != string::npos)
            {
                // RANGE
                istringstream (line.substr(6)) >> value1 >> value2;
                // cout << query_type[3] << " " << value1  << " " <<  value2 << '\n';
            }
            else
            {
                cout << "Command Not Found\n";
            }
        }
    }
        
}