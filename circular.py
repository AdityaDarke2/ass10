class CNode:
    def __init__(self, key, data):
        self.key = key
        self.data = data
        self.next = None

class CircularLinkedList:
    def __init__(self):
        self.head = None

    def insert_end(self, key, data):
        new_node = CNode(key, data)
        if self.head is None:
            self.head = new_node
            new_node.next = self.head
            return
        temp = self.head
        while temp.next != self.head:
            temp = temp.next
        temp.next = new_node
        new_node.next = self.head

    def display(self):
        if self.head is None:
            print("Empty list")
            return
        temp = self.head
        while True:
            print(f"[{temp.key}: {temp.data}]", end=" -> ")
            temp = temp.next
            if temp == self.head:
                break
        print("(HEAD)")

    def delete(self, key):
        if self.head is None:
            return
        # Case 1: Deleting head
        if self.head.key == key:
            if self.head.next == self.head:         # Only 1 node in list
                self.head = None
            else:
                temp = self.head
                while temp.next != self.head:       # Find last node
                    temp = temp.next
                temp.next = self.head.next
                self.head = self.head.next
            return
        # Case 2: Deleting non-head node
        temp = self.head
        while temp.next != self.head:
            if temp.next.key == key:
                temp.next = temp.next.next
                return
            temp = temp.next
