#include <bits/stdc++.h>
using namespace std;

int N = 3;
int leaf_keys = ceil((float)(N+1)/(float)2);
int non_leaf_keys = ceil((float)(N)/(float)2);     // size for non-leaf values split.
int non_leaf_ptrs = ceil((float)(N+2)/(float)2);
int maxx = 1e9+1;

bool DM = false;

typedef struct node{

    node* parent;
    vector<int> keys;
    vector<node*> pointers;
    int isLeaf;
    node* next_pointer;

}Node;

Node* init_node(int is_leaf,Node* parent)
{

    Node* node = new Node;
    node->parent = parent;
    node->keys = vector<int>(N,maxx);
    node->pointers = vector<Node*>(N+1);
    node->isLeaf = is_leaf;
    node->next_pointer = NULL;
    return node;
}

int find_lower_index(Node* node, int value)
{
    // finds the index which has a value just less than the given value.
    // bascially (lower_bound - 1)
    auto it = lower_bound(node->keys.begin(),node->keys.end(),value);
    int ind = it - node->keys.begin();
    return ind;
}

void insert(Node* node,int value,Node* new_node)
{
    // root is returned from the function
    // node in which we have to insert will be a leaf
    if(node->keys[N-1] == maxx)
    {
        // means there is still space in this leaf node
        // therefore find the first index which is empty, and then sort it.
        int ind = find_lower_index(node,value);
        node->keys.pop_back();
        node->pointers.pop_back();
        node->keys.insert(node->keys.begin()+ind,value);


        if (node->isLeaf != 1)
        {
            node->pointers.insert(node->pointers.begin()+ind+1,new_node);
        }
        
        cout << "Key with value " << value << " inserted sucessfully\n";
        // cout << node->keys.size() << node->keys[0] << endl;

        // for(int i=0;i<node->keys.size();i++)
        // {
        //     cout << i << " ";
        //     if(node->keys[i] != maxx)
        //     {
        //         cout << node->keys[i];
        //     }
        //     cout << endl;
        // }
    }
    else
    {
        // there is no space in leaf. so split the root - call the split function
        // since there is no space , we make a vector with the keys in temp and sort the values
        auto it = lower_bound(node->keys.begin(),node->keys.end(),value);
        int ind = it - node->keys.begin()+1;
        cout << "ind " << ind << endl;
        int up_value;

        vector<int> temp_vals;
        vector<Node*> temp_ptrs;

        temp_vals.push_back(value);
        for(int i=0;i<N;i++)
        {
            temp_vals.push_back(node->keys[i]);       
        }
        sort(temp_vals.begin(),temp_vals.end());

        for(int i=0;i<N+1;i++)
        {
            temp_ptrs.push_back(node->pointers[i]);       
            if(i == ind-1)
            {
                temp_ptrs.push_back(new_node);
            }
        }

        // for(int i=0;i<=N;i++)
        // {
        //     cout << temp_vals[i] << " ";
        // }
        // cout << '\n';

        Node* split_node1 = init_node(node->isLeaf,node->parent);
        Node* split_node2 = init_node(node->isLeaf,node->parent);

        //  first ciel(n+1/2) values in one and rest in other

        if (node->isLeaf != 1)
        {
            for(int i=0;i<non_leaf_keys;i++)
            {
                split_node1->keys[i] = temp_vals[i];
            }
            for(int i=non_leaf_keys;i<temp_vals.size();i++)
            {
                split_node2->keys[i-non_leaf_keys] = temp_vals[i];
            }

            for(int i=0;i<non_leaf_ptrs;i++)
            {
                split_node1->pointers[i] = temp_ptrs[i];
            }    
            for(int i=0;i<non_leaf_ptrs;i++)
            {
                split_node2->pointers[i-non_leaf_ptrs] = temp_ptrs[i];
            }

            up_value = split_node2->keys[0];
            split_node2->keys.erase(split_node2->keys.begin());
            
        }
        else
        {
            for(int i=0;i<leaf_keys;i++)
            {
                split_node1->keys[i] = temp_vals[i];
            }
            for(int i=leaf_keys;i<temp_vals.size();i++)
            {
                split_node2->keys[i-leaf_keys] = temp_vals[i];
            }
            
            up_value = split_node2->keys[0];

            // for(int i=0;i<split_node1->keys.size();i++)
            // {
            //     cout << split_node1->keys[i] << " ";
            // }
            // cout << endl;

            // for(int i=0;i<split_node2->keys.size();i++)
            // {
            //     cout << split_node2->keys[i] << " ";
            // }
            // cout << endl;
            
        }
        
        split_node2->next_pointer = split_node1->next_pointer;
        split_node1->next_pointer = split_node2;

        // cout << "SegNO\n";
        //  first ciel(n+2/2) values in one and rest in other   
        *node = *split_node1;
        *new_node = *split_node2;
        // cout << "SegNO1\n";
        // for(int i=0;i<node->keys.size();i++)
        //     {
        //         cout << node->keys[i] << " ";
        //     }
        //     cout << endl;

        //     for(int i=0;i<new_node->keys.size();i++)
        //     {
        //         cout << new_node->keys[i] << " ";
        //     }
        //     cout << endl;
        
        if (node->parent != NULL)
        {
            cout << "blah\n";
            insert(node->parent,up_value,new_node);
        }
        return;
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

Node* insert_val(Node* root,int value)
{
    /// Returns root of the tree
    Node* temp = search(root,value);
    // cout << temp->keys.size() << endl;
    // temp will be a leaf node.
    // what if temp returns null. can it return null? , yes if node is not found. , but we do not want null here and hence the search function never returns a null
    Node * new_node = init_node(1,NULL);
    insert(temp,value,new_node);

    if(temp->parent == NULL)
    {
        
    }
    
    // for(int i=0;i<temp->keys.size();i++)
    // {
    //     if(temp->keys[i] != maxx)
    //     {
    //         cout << temp->keys[i] << " ";
    //     }
    // }
    // cout << '\n';

    // for(int i=0;i<new_node->keys.size();i++)
    // {
    //     if(new_node->keys[i] != maxx)
    //     {
    //         cout << new_node->keys[i] << " ";
    //     }
    // }
    // cout << '\n';

    return root;
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
                root = insert_val(root,value1);
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