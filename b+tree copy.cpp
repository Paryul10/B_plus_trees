#include <bits/stdc++.h>
using namespace std;

int N = 3;
int maxx = 1e9+1;

bool DM = false;

typedef struct node{

    node* parent;
    vector<int> keys;
    vector<node*> pointers;
    int isLeaf;

}Node;

Node* init_node(int is_leaf,Node* parent)
{
    Node* node = new Node;
    node->parent = parent;
    node->keys = vector<int>(N,maxx);
    node->pointers = vector<Node*>(N+1);
    node->isLeaf = is_leaf;
    return node;
}

// returns index at which to store , otherwise -1
// int check_empty(Node* node)
// {
//     int return_val = -1;
//     int ind = 0;
//     while(ind < N)
//     {
//         if (node->keys[ind] == maxx)
//         {
//             return_val = ind;
//             break;
//         }
//         ind++;
//     }

//     return return_val;
// }

int find_lower_index(Node* node, int value)
{
    auto it = lower_bound(node->keys.begin(),node->keys.end(),value);
    int ind = it - node->keys.begin() - 1;
    if (ind == -1)
    {
        // insert at the first postition.
        ind = 0;
    }

    return ind;
}

Node* insert(Node* node,int value)
{
    // root is returned from the function

    // node is leaf
    if(node->isLeaf == 1)
    {
        if(node->keys[N-1] == maxx)
        {
            // means there is still space in this node
            // therefore find the first index which is empty, and then sort it.
            int ind = find_lower_index(node,maxx);
            sort(node->keys.begin(),node->keys.end());

            if (ind == 0)  // the first entry of the paresnt node should change , so that minimum value is pointed to
            {
                node->parent[keys][0] = value;
            }
            
            cout << "Key with value " << value << " inserted sucessfully\n";

            // if (DM == true)
            // {
            //     int ind = 0;
            //     while(ind < N)
            //     {
            //         if (node->keys[ind] == maxx)
            //         {
            //             node->keys[ind] = value;
            //         }
            //         ind++;
            //     }
            // }
        }
        else
        {
            // there is no space in leaf. so split the root - call the split function

        }
    }
    else
    {
        // search where to insert. - call the search function
        
    }
    
}


Node* search(Node* node,int value)
{
    if (node->isLeaf == 1)
    {
        if (binary_search(node->keys.begin(),node->keys.end(),value) == 1)
        {
            cout << "Element with value " << value << " exists in the tree\n";
        }
        else
        {
            cout << "Element with value " << value << " not found\n";
        }
        return node;   // returns node pointer where the value is stored.
    }
    else
    {
        // vector<int>::iterator it = upper_bound(node->keys.begin(),node->keys.end(),value);                 // auto it;
        auto it = upper_bound(node->keys.begin(),node->keys.end(),value);
        int ind = it - node->keys.begin();
        node = node->pointers[ind];
        search(node,value);
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
        Node* root = init_node(1,NULL);
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