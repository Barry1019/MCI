public class BinarySearch extends MySearchAlg{
	public int search(int[] array, int num){

		int[] sortedArray = sort(array);
		int[] indexMap = indexMap(array);
		int n = array.length;

		int currentIndex = this.searchBS(sortedArray, num, 0, n-1);
		if (currentIndex != -1) {
			int originalIndex = indexMap[currentIndex];
			return originalIndex;
		}else{
			return -1;
		}
	}

	//  search a sorted array recursively
	private int searchBS(int[] array, int num, int start, int end){
		int middle = Math.round((start+end) / 2.0f);
		//  base case 1
		if (start > end) {
			return -1;
		}
		//  base case 2
		if (array[middle] == num) {
			return middle;
		}else if (array[middle] < num) {  //  use the left part
			return this.searchBS(array, num, start, middle-1);
		}else{  //  use the right part
			return this.searchBS(array, num, middle+1, end);
		}
	}
}