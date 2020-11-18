public abstract class MySearchAlg{
	abstract int search(int[] array, int num);

	//  a method to sort an array in ascending order without changing the original one
	public static int[] sort(int[] array){
		int n = array.length;
		//  make a copy of the original array
		int[] sortedArray = new int[n];
		for (int i = 0; i < n; i++) {
			sortedArray[i] = array[i];
		}

		for (int i = 1; i < n; i++) {
			int key = sortedArray[i];
			int j = i - 1;

			while(j >= 0 && sortedArray[j] > key){
				sortedArray[j+1] = sortedArray[j];
				j--;
			}
			sortedArray[j+1] = key;
		}

		return sortedArray;
	}

	//  a method to return an index map
	public static int[] indexMap(int[] array){
		int n = array.length;
		int[] indexMap = new int[n];
		
		//  make a copy of the original array
		int[] sortedArray = new int[n];
		for (int i = 0; i < n; i++) {
			sortedArray[i] = array[i];
		}

		//  create an array of original indexes
		for (int i = 0; i < n; i++) {
			indexMap[i] = i;
		}

		for (int i = 1; i < n; i++) {
			int key = sortedArray[i];
			int j = i - 1;

			while(j >= 0 && sortedArray[j] > key){
				sortedArray[j+1] = sortedArray[j];
				indexMap[j+1] = indexMap[j];
				j--;
			}
			indexMap[j+1] = i;
		}

		return indexMap;
	}
}