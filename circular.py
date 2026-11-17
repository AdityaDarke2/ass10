class Node:
    def __init__(self, song_id, song_name):
        self.song_id = song_id
        self.song_name = song_name
        self.next = None

class CircularLinkedList:
    def __init__(self):
        self.head = None

    def addSong(self, song_id, song_name):
        new_node = Node(song_id, song_name)
       
        if self.head is None:
            self.head = new_node
            new_node.next = self.head
            return

        temp = self.head
        while temp.next != self.head:
            temp = temp.next
           
        temp.next = new_node
        new_node.next = self.head

    def displayPlaylist(self):
        if self.head is None:
            print("The playlist is empty.")
            return

        temp = self.head
       
        while True:
            print(f"{temp.song_id} {temp.song_name} ")
            temp = temp.next
            if temp == self.head:
                break


        print("\n")

    def deleteSong(self, song_id):
        if self.head is None:
            print("Cannot delete. The playlist is empty. \n")
            return
           
        if self.head.song_id == song_id:
            if self.head.next == self.head:
                self.head = None
            else:                
                temp = self.head
                while temp.next != self.head:
                    temp = temp.next
                   
                temp.next = self.head.next
                self.head = self.head.next
               
            print(f"Deleted song with ID: {song_id} \n")
            return
           
        temp = self.head
        while temp.next != self.head:
            if temp.next.song_id == song_id:
                temp.next = temp.next.next
                print(f"Deleted song with ID: {song_id} \n")
                return
            temp = temp.next
           
        print(f"Cannot delete. Song with ID {song_id} was not found. \n")

if __name__ == "__main__":
    playlist = CircularLinkedList()
    playlist.addSong(101, "Blinding Lights")
   
    playlist.displayPlaylist()

    playlist.deleteSong(102)
   
    playlist.displayPlaylist()
   
