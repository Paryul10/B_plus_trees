#include <bits/stdc++.h>
using namespace std;

int N = 3;
int leaf_keys = ceil((float)(N + 1) / (float)2);
int non_leaf_keys = ceil((float)(N) / (float)2); // size for non-leaf values split.
int non_leaf_ptrs = ceil((float)(N + 2) / (float)2);
int maxx = 1e9 + 1;

bool DM = false;

typedef struct node
{

    node *parent;
    vector<int> keys;
    vector<node *> pointers;
    int isLeaf;
    node *next_pointer;
    vector<int> counts;

} Node;

Node *init_node(int is_leaf, Node *parent)
{

    Node *node = new Node;
    node->parent = parent;
    node->keys = vector<int>(N, maxx);
    node->pointers = vector<Node *>(N + 1);
    node->isLeaf = is_leaf;
    node->next_pointer = NULL;
    if (node->isLeaf == 1)
    {
        node->counts = vector<int>(N, 0);
    }
    return node;
}

int find_lower_index(Node *node, int value)
{
    // finds the index which has a value just less than the given value.
    // bascially (lower_bound - 1)
    auto it = lower_bound(node->keys.begin(), node->keys.end(), value);
    int ind = it - node->keys.begin();
    return ind;
}

Node *insert(Node *node, int value)
{
    Node *split_node = NULL;
    if (node->isLeaf == 1)
    {
        // LEAF
        if (node->keys[N - 1] == maxx)
        {
            // If leaf is empty             
            auto it = lower_bound(node->keys.begin(), node->keys.end(), value);
            int ind = it - node->keys.begin();
            if (*it == value)
            {
                // if value already there , then just increase count
                node->counts[ind] += 1;
            }
            else
            {
                // otherwise add it to the specified index
                node->keys.pop_back();
                node->keys.insert(node->keys.begin() + ind, value);
                node->counts.pop_back();
                node->counts.insert(node->counts.begin() + ind, 1);
            }
            return NULL;
        }
        else
        {
            // no empty space in leaf 
            auto it = lower_bound(node->keys.begin(), node->keys.end(), value);
            if (*it == value)
            {
                // if already exists then increment val
                int ind = it - node->keys.begin();
                node->counts[ind] += 1;
                return NULL;
            }
            else
            {
                // otherwise split leaf
                pair<int, int> temp_vals[N + 1];
                for (int i = 0; i < N; i++)
                {
                    temp_vals[i].first = node->keys[i];
                    temp_vals[i].second = node->counts[i];
                }
                temp_vals[N].first = value;
                temp_vals[N].second = 1;
                sort(temp_vals, temp_vals + N + 1);
                int up_value;
                Node *split_node1 = init_node(node->isLeaf, node->parent);
                Node *split_node2 = init_node(node->isLeaf, node->parent);

                for (int i = 0; i < leaf_keys; i++)
                {
                    split_node1->keys[i] = temp_vals[i].first;
                    split_node1->counts[i] = temp_vals[i].second;
                }
                for (int i = leaf_keys; i < N + 1; i++)
                {
                    split_node2->keys[i - leaf_keys] = temp_vals[i].first;
                    split_node2->counts[i - leaf_keys] = temp_vals[i].second;
                }

                // split_node2->next_pointer = split_node1->next_pointer;
                // split_node1->next_pointer = split_node2;
                split_node1->next_pointer = node->next_pointer;
                memcpy(node, split_node1, sizeof *split_node1);
                // cout << "node ka next" << '\n';
                // for(int i=0;i<N;i++)
                // {
                //     cout << node->keys[i] << " ";
                // }
                // cout << '\n';

                // cout << "split_node2" << '\n';
                // for(int i=0;i<N;i++)
                // {
                //     cout << split_node2->keys[i] << " ";
                // }
                // cout << '\n';


                split_node2->next_pointer = node->next_pointer;
                node->next_pointer = split_node2;

                // cout << "node->next" << '\n';
                // for(int i=0;i<N;i++)
                // {
                //     cout << node->next_pointer->keys[i] << " ";
                // }
                // cout << '\n';

                // cout << "split_node2->next" << '\n';
                // for(int i=0;i<N;i++)
                // {
                //     if (split_node2->next_pointer != NULL)
                //         cout << split_node2->next_pointer->keys[i] << " ";
                //     else
                //     {
                //         cout << "Next is null" << " ";
                //     }
                // }
                // cout << '\n';

                return split_node2;
            }
        }
    }
    auto it = upper_bound(node->keys.begin(), node->keys.end(), value);
    int ind = it - node->keys.begin();
    Node *return_node = insert(node->pointers[ind], value);

    if (return_node)
    {
        // if some splitted node is returned then add it to the parent
        int up_value = return_node->keys[0];
        if (return_node->isLeaf == 0)
        {
            return_node->keys.erase(return_node->keys.begin());
            return_node->keys.push_back(maxx);
        }

        if (node->keys[N - 1] == maxx)
        {
            auto it = upper_bound(node->keys.begin(), node->keys.end(), up_value);
            int ind = it - node->keys.begin();

            node->keys.pop_back();
            node->pointers.pop_back();
            node->keys.insert(node->keys.begin() + ind, up_value);
            node->pointers.insert(node->pointers.begin() + ind + 1, return_node);
            return NULL;
        }
        else
        {
            auto it = lower_bound(node->keys.begin(), node->keys.end(), up_value);
            int ind = it - node->keys.begin();
            vector<int> temp_vals;
            vector<Node *> temp_ptrs;

            temp_vals.push_back(up_value);
            for (int i = 0; i < N; i++)
            {
                temp_vals.push_back(node->keys[i]);
            }
            sort(temp_vals.begin(), temp_vals.end());

            for (int i = 0; i < N + 1; i++)
            {
                temp_ptrs.push_back(node->pointers[i]);
                if (i == ind)
                {
                    temp_ptrs.push_back(return_node);
                }
            }
            
            Node *split_node1 = init_node(node->isLeaf, node->parent);
            Node *split_node2 = init_node(node->isLeaf, node->parent);
            for (int i = 0; i < non_leaf_keys; i++)
            {
                split_node1->keys[i] = temp_vals[i];
            }
            for (int i = non_leaf_keys; i < temp_vals.size(); i++)
            {
                split_node2->keys[i - non_leaf_keys] = temp_vals[i];
            }

            for (int i = 0; i < non_leaf_ptrs; i++)
            {
                split_node1->pointers[i] = temp_ptrs[i];
            }
            for (int i = non_leaf_ptrs; i < temp_ptrs.size(); i++)
            {
                split_node2->pointers[i - non_leaf_ptrs] = temp_ptrs[i];
            }

            split_node = split_node2;
            memcpy(node, split_node1, sizeof *split_node1);
            return split_node;
        }
    }
    return return_node;
}

Node *search(Node *node, int value, int pr)
{
    if (node->isLeaf == 1)
    {
        if (binary_search(node->keys.begin(), node->keys.end(), value) == 1)
        {
            if (pr == 1)
                cout << "YES\n";
        }
        else
        {
            if (pr == 1)
                cout << "NO\n";
        }

        return node; // returns node pointer where the value is stored.
    }
    else
    {
        // vector<int>::iterator it = upper_bound(node->keys.begin(),node->keys.end(),value);                 // auto it;
        auto it = upper_bound(node->keys.begin(), node->keys.end(), value);
        int ind = it - node->keys.begin();
        node = node->pointers[ind];
        return search(node, value, pr);
    }
}

Node *insert_val(Node *root, int value)
{
    Node *split_node = insert(root, value);
    if (split_node)
    {
        Node *new_node = init_node(0, NULL);
        new_node->keys[0] = split_node->keys[0];
        if (split_node->isLeaf == 0)
        {
            split_node->keys.erase(split_node->keys.begin());
            split_node->keys.push_back(maxx);
        }
        new_node->pointers[0] = root;
        new_node->pointers[1] = split_node;
        root = new_node;
    }
    return root;
}

void print(node *node, int level)
{
    cout << "Level " << level << ":";
    for (int i = 0; i < N; i++)
    {
        if (node->keys[i] != maxx)
            cout << node->keys[i] << " ";
    }
    cout << endl;
    if (node->isLeaf == 0)
    {
        for (int i = 0; i < N + 1; i++)
        {
            if (node->pointers[i] != NULL)
                print(node->pointers[i], level + 1);
        }
    }
}

int count(Node *root, int val)
{
    Node *node = search(root, val, 0);
    int total = 0;
    for (int i = 0; i < N; i++)
    {
        if (node->keys[i] == val)
            total += node->counts[i];
    }
    return total;
}

int range(Node *root, int x, int y)
{
    int total = 0;
    
    // if nothing works
    Node* node = search(root,x,0);
    while(node != NULL)
    {
        for(int i=0;i<N;i++)
        {
            if (node->keys[i] >=x && node->keys[i] <=y)
            {
                total += node->counts[i];
            }
        }
        // cout << node->keys[0] << " " << node->keys[1] << " "  << node->keys[2] << "\n";
        // cout << node->counts[0] << " " << node->counts[1] << " "  << node->counts[2] << "\n";
        node = node->next_pointer;
        // cout << node->keys[0] << " " << node->keys[1] << " "  << node->keys[2] << "\n";
        // cout << node->counts[0] << " " << node->counts[1] << " "  << node->counts[2] << "\n";
    }
    return total;
}

int main(int argc, char const *argv[])
{
    string query_type[4] = {"FIND", "INSERT", "COUNT", "RANGE"};
    string filename;

    if (argc <= 1)
    {
        cout << "Filename not given\n";
    }
    else
    {
        Node *root = init_node(1, NULL);
        filename = argv[1];
        ifstream file(filename);
        string line;
        int value1, value2;
        while (getline(file, line))
        {

            if (line.find(query_type[0]) != string::npos)
            {
                // FIND
                istringstream(line.substr(5)) >> value1;
                search(root, value1, 1);
            }
            else if (line.find(query_type[1]) != string::npos)
            {
                // INSERT
                istringstream(line.substr(7)) >> value1;
                root = insert_val(root, value1);
            }
            else if (line.find(query_type[2]) != string::npos)
            {
                // COUNT
                istringstream(line.substr(6)) >> value1;
                int ans = count(root, value1);
                cout << ans << endl;
            }
            else if (line.find(query_type[3]) != string::npos)
            {
                // RANGE
                istringstream(line.substr(6)) >> value1 >> value2;
                int ans = range(root, value1, value2);
                cout << ans << endl;
            }
            else
            {
                cout << "Command Not Found\n";
            }
            // print(root, 1);
        }
    }
}