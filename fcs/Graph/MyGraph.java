//student ID: a1790846
//student name: Shaokang Ma

import java.util.LinkedList;

public class MyGraph {
	//  attribute
	LinkedList<Node> adjListArray[]; 

	// initialise an empty graph in the constructor.
	public MyGraph() {

	}

	// transform an adjacency matrix to an adjacency list
	@SuppressWarnings("unchecked")
	public void matrixToList(int[][] matrix) {
		//  check the number of vertices
		int n = matrix.length;
		//  initialise the adjListArray
		this.adjListArray = new LinkedList[n];

		for (int i = 0; i < n; i++) {
			this.adjListArray[i] = new LinkedList<Node>();
			for (int j = 0; j < n; j++) {
				if (matrix[i][j] == 1) {
					this.adjListArray[i].add(new Node(j));
				}
			}
		}
	}

	// Print out the adjacency list array
	public void displayAdjListArray() {
		if (this.adjListArray == null) {
			System.out.println("There is no path at all.");
			return;
		}  //  in case the list is empty

		int n = this.adjListArray.length;

		for (int i = 0; i < n; i++) {
			System.out.print(i + ": ");
			//  in case the list is empty
			if (this.adjListArray[i] == null) {
				System.out.println("There is no edge to this node.");
			}else{
				//  Print out the first element
				this.adjListArray[i].get(0).printNode();
				//  ensure the format
				int size = this.adjListArray[i].size();
				for (int j = 1; j < size; j++) {
					System.out.print(" -> ");
					this.adjListArray[i].get(j).printNode();
				}
				System.out.println();
			}
		}
	}
}

