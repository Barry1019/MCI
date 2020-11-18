//student ID: a1790846
//student name: Shaokang Ma

public class InsertionSort extends MySortAlg{

	public int[] sort(int[] array){
		int n = array.length;

		for (int i = 1; i < n; i++) {
			int key = array[i];
			int j = i - 1;

			while(j >= 0 && this.compareInt(array[j], key, true) == 1){
				array[j+1] = array[j];
				j--;
			}

			array[j+1] = key;
		}

		return array;
	}
}