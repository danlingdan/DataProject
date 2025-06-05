#define OK 1
#define ERROR 0

#include <iostream>

#define Status int

typedef struct LNode {
	int data;
	struct LNode *next;
}LNode,*LinkList;

// Function to initialize a linked list
Status InitList(LNode& L) {
	LNode *p = new LNode; // Create a new node
	if (!p) return ERROR; // Check if memory allocation was successful
	p->next = nullptr; // Initialize the next pointer to nullptr
	L = *p; // Assign the new node to the list head
	return OK; // Return success status
}

// Destroy the linked list and free memory
Status DestroyList(LNode& L) {
	LNode *p = L.next; // Start from the first node
	while (p) {
		LNode *temp = p; // Store the current node
		p = p->next; // Move to the next node
		delete temp; // Delete the current node
	}
	L.next = nullptr; // Set the head's next pointer to nullptr
	return OK; // Return success status
}

Status ListInsert_Head(LNode& L, int e) {
	LNode* s = new LNode;
	if (!s) return ERROR;
	s->data = e;
	s->next = L.next;
	L.next = s;
	return OK;
}

Status ListInsert_Tail(LNode& L, int e) {
	LNode* p = &L;
	while (p->next) {
		p = p->next;
	}
	LNode* s = new LNode;
	if (!s) return ERROR;
	s->data = e;
	s->next = nullptr;
	p->next = s;
	return OK;
}

int main() {
	LNode L;
	if (InitList(L) == OK) {
		std::cout << "List initialized successfully." << std::endl;
	} else {
		std::cout << "Failed to initialize list." << std::endl;
		return 1;
	}
	ListInsert_Head(L, 10);
	ListInsert_Tail(L, 20);
	ListInsert_Head(L, 5);
	LNode* p = L.next; // Start from the first node
	while (p) {
		std::cout << p->data << " "; // Print the data in each node
		p = p->next; // Move to the next node
	}
	std::cout << std::endl;
	if (DestroyList(L) == OK) {
		std::cout << "List destroyed successfully." << std::endl;
	} else {
		std::cout << "Failed to destroy list." << std::endl;
	}
	return 0;
}