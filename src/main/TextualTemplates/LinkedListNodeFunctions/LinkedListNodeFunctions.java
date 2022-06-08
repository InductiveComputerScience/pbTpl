package TextualTemplates.LinkedListNodeFunctions;


import TextualTemplates.TextualTemplates.Node;

public class LinkedListNodeFunctions {
	public static LinkedListNodes CreateLinkedListNodes(){
		LinkedListNodes ll;

		ll = new LinkedListNodes();
		ll.first = new LinkedListNodeNodes();
		ll.last = ll.first;
		ll.last.end = true;

		return ll;
	}

	public static void LinkedListAddNode(LinkedListNodes ll, Node value){
		ll.last.end = false;
		ll.last.value = value;
		ll.last.next = new LinkedListNodeNodes();
		ll.last.next.end = true;
		ll.last = ll.last.next;
	}

	public static Node [] LinkedListNodesToArray(LinkedListNodes ll){
		Node [] array;
		double length, i;
		LinkedListNodeNodes node;

		node = ll.first;

		length = LinkedListNodesLength(ll);

		array = new Node [(int)(length)];

		for(i = 0d; i < length; i = i + 1d){
			array[(int)(i)] = node.value;
			node = node.next;
		}

		return array;
	}

	public static double LinkedListNodesLength(LinkedListNodes ll){
		double l;
		LinkedListNodeNodes node;

		l = 0d;
		node = ll.first;
		for(; !node.end; ){
			node = node.next;
			l = l + 1d;
		}

		return l;
	}

	public static void FreeLinkedListNode(LinkedListNodes ll){
		LinkedListNodeNodes node, prev;

		node = ll.first;

		for(; !node.end; ){
			prev = node;
			node = node.next;
			delete(prev);
		}

		delete(node);
	}

  public static void delete(Object object){
    // Java has garbage collection.
  }
}
