//student ID: a1790846
//student name: Shaokang Ma

public abstract class MySortAlg{

	abstract int[] sort(int[] array);

	//  a method to swap two elements
	public void swap(int[] array, int i, int j){
		int tmp = array[i];
		array[i] = array[j];
		array[j] = tmp;
	}

	//  a method to display an array
	public void printArray(int[] array){
		System.out.print("[ ");
		for (int i = 0;i < array.length ;i++ ) {
			System.out.print(array[i]);
			System.out.print(" ");
		}
		System.out.println("]");
	}

	//  a method to compare 2 elements
	public int compareInt(int i, int j, boolean descending){
		if (i == j) {
			return 0;
		}else if (i < j) {
			if (descending) {
				return 1;
			}else{
				return -1;
			}
		}else{
			if (descending) {
				return -1;
			}else{
				return 1;
			}
		}
	}
}