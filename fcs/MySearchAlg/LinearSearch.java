public class LinearSearch extends MySearchAlg{
	public int search(int[] array, int num){
		int n = array.length;
		int pos = -1;

		for (int i = 0; i < n; i++) {
			if (array[i] == num) {
				pos = i;
				break;
			}
		}
		return pos;
	}
}