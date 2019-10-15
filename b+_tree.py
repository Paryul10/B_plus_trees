from __future__ import print_function
import sys
import time
import os
import numpy as np
import bisect

def print_tree(node):
    
    for pointers in node.pointers:
        if node.is_leaf == 0:
            print_tree(pointers)
        else:
            for key in node.keys:
                print(key,end = " ")


def get_query_leaf(node,value):
	num_keys = len(node.keys)-1
	if node.is_leaf == 0:
		if node.keys[0] >= value:
			return get_query_leaf(node.pointers[0],value)
		if node.keys[num_keys] < value:
			return get_query_leaf(node.pointers[num_keys],value)
		for idx in range(num_keys):
			if value <= node.keys[idx+1]:
				if value > node.keys[idx]:
					return get_query_leaf(node.pointers[idx+1],value)
	elif node.is_leaf == 1:
		return node

def get_keys(node,min_value,max_value):
    key_count = 0
    num_keys = len(node.keys)
    if (num_keys == 0):
        return None,0
    
    for i in range(num_keys):
        if node.keys[i] >= min_value and node.keys[i] <= max_value:
            key_count = key_count + 1
    
    if node.keys[num_keys-2] <= max_value:
        return node.next,key_count
    if node.keys[num_keys-2] > max_value:
        return None,key_count

def count_range(tree,min_value,max_value):
    count = 0
    leaf = get_query_leaf(tree.root,min_value)
    while leaf is not None:
        leaf,count_leaf = get_keys(leaf,min_value,max_value)
        count += count_leaf
    return count

def split(node):
    sib_node = Node()
    sib_node.is_leaf = node.is_leaf
    num_keys = len(node.keys)
    mid = num_keys // 2
    mid_val = node.keys[mid]

    if node.is_leaf == 1:
        sib_node.keys = node.keys[mid:]
        sib_node.pointers = node.pointers[mid:]
        sib_node.next = node.next

        node.keys = node.keys[:mid]
        node.pointers = node.pointers[:mid]
        node.next = sib_node
    else:
        sib_node.keys = node.keys[mid+1:]
        sib_node.pointers = node.pointers[mid+1:]
        node.keys = node.keys[:mid]
        node.pointers = node.pointers[:mid+1]
    
    return mid_val,sib_node

def check_overflow(node,N):
    num_keys = len(node.keys)
    if num_keys > N:
        ## if total number is greater than what it can hold then split.
        return 1
    else:
        ## just added at that index, no change to be made
        return 0

def mid_check(node,mid,newNode,N):
    if mid is None:
        return None,None
    else:
        insert_index = bisect.bisect(node.keys,mid)
        node.keys.insert(insert_index,mid)
        node.pointers.insert(insert_index+1,newNode)
        if len(node.keys) > N:
            ## if total number is greater than what it can hold then split.
            return split(node)
        else:
            ## just added at that index, no change to be made
            return None,None

class Node:
    def __init__(self):
        self.keys = []
        self.pointers = []
        self.is_leaf = 1
        self.next = None
    
class Bplus_Tree():

    def __init__(self,N):
        
        self.root = Node()
        self.keys = []
        self.pointers = []
        self.is_leaf = 1
        self.num = N
        self.next = None
    
    def insert(self,node,value):

        num_keys = len(node.keys)
        flag = 0
        if not node.is_leaf:
            for i in range(num_keys):
                if flag == 1:
                    ## element has been inserted
                    break
                
                if value < node.keys[i] and i == 0:
                    ## value to be inserted at first position
                    mid , newNode = self.insert(node.pointers[0],value)
                    flag = 1
                elif value >= node.keys[i] and i == num_keys-1:
                    ## insert at the last
                    mid , newNode = self.insert(node.pointers[-1],value)
                    flag = 1
                elif value < node.keys[i+1] and value >= node.keys[i]:
                    ## insert at the i+1 th position
                    mid , newNode = self.insert(node.pointers[i+1],value)
                    flag = 1

            
        else:
            ## if not leaf
            insert_index = bisect.bisect(node.keys,value)
            node.keys.insert(insert_index,value)
            node.pointers.insert(insert_index,value)
            if len(node.keys) > self.num:
                ## if total number is greater than what it can hold then split.
                return split(node)
            else:
                ## just added at that index, no change to be made
                return None,None

            
        ## will have to send self.num because class variable.    
        return mid_check(node,mid,newNode,self.num)

def split_node(node):
	if node.is_leaf == 1:
		newNode = Node()
		newNode.is_leaf = node.is_leaf
		n = len(node.keys)
		mid = n // 2
		mid_value = node.keys[mid]
		newNode.keys = node.keys[mid:]
		newNode.pointers = node.pointers[mid:]
		newNode.next = node.next
		node.keys = node.keys[:mid]
		node.pointers = node.pointers[:mid]
		node.next = newNode
	else:
		newNode = Node()
		newNode.is_leaf = node.is_leaf
		n = len(node.keys)
		mid = n // 2
		mid_value = node.keys[mid]
		newNode.keys = node.keys[mid+1:]
		newNode.pointers = node.pointers[mid+1:]
		node.keys = node.keys[:mid]
		node.pointers = node.pointers[:mid+1]
	
	return mid_value, newNode

def main():
    
    with open(file_path,'r') as file:
        for line in file:
            query = line.strip().split()
            
            
            if len(query) == 2:
                val1 = query[1]
            
            if len(query) == 3:
                val1 = query[1]
                val2 = query[2]
            
            if(query[0] == "INSERT"):
                ## insert val1 into tree
                # mid,newNode = tree.insert(tree.root,val1)
                mid,newNode = tree.insert(tree.root,val1)

                if mid is not None:
                    new_root = Node()
                    new_root.keys = [mid]
                    new_root.is_leaf = 0
                    new_root.pointers = [tree.root,newNode]
                    tree.root = new_root
                
                print("-------Tree------")
                print_tree(tree.root)
                print()
                print("-------Treeend------")

            
            elif(query[0] == "FIND"):
                ## find val1 in tree
                count = count_range(tree,val1,val1)
                if count == 0:
                    print("NO")
                else:
                    print("YES")
            elif(query[0] == "COUNT"):
                ## count of val1 
                count = count_range(tree,val1,val1)
                print(count)
            elif(query[0] == "RANGE"):
                ## number of elements in range val1 to val2
                count = count_range(tree,val1,val2)
                print(count)
            else:
                print("Invalid Query")
            
            
if __name__ == "__main__":

    if len(sys.argv) < 2:
        print("Usage: python3 b+tree.py 'path_to_input_file';")
        sys.exit(1)
    else:
        N = 3
        command=sys.argv
        file_path = command[1]
        tree = Bplus_Tree(N)
        main()
