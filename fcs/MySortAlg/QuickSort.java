public class QuickSort extends MySortAlg{
	public int[] sort(int[] array){
		int n = array.length;
		this.quickSort(array, 0, n-1);
		return array;
	}

	private void quickSort(int[] array, int start, int end){
		int part = separate(array, start, end);

		if (part - 1 > start) {
			quickSort(array, start, part - 1);
		}
		if (part + 1 < end) {
			quickSort(array, part + 1, end);
		}
	}

	//  A method to separate an array into 2 parts
	private int separate(int[] array, int start, int end){
		int pivot = array[end];

		//  put all the smaller elements into the left part
		for (int j = start; j < end; j++) {
			if (array[j] > pivot) {
				swap(array, j, start);
				start++;
			}
			
		}
		int temp = array[start];
		array[start] = pivot;
		array[end] = temp;

		return start;
	}
}