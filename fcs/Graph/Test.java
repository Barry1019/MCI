import java.util.LinkedList;

public class Test {
	public static void main(String[] args) {
		MyGraph m1 = new MyGraph();
		//  input the matrix according to the requirement
		int[][] matrix = {{0,1,0,1}, {1,0,0,0}, {0,0,0,1}, {0,1,1,0}};

		m1.matrixToList(matrix);
		m1.displayAdjListArray();
	}
}