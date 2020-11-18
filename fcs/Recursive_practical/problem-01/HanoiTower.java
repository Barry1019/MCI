//==================================
// Foundations of Computer Science
// Student: Shaokang Ma
// id: a1790846
// Semester: 1
// Year: 2020
// Practical Number: 04 - Part II
//===================================

public class HanoiTower{
	//  attribute
	private int disks;

	//  Constructor
	HanoiTower(){
		this.setDisks(3);
	}
	HanoiTower(int dTmp){
		this.setDisks(dTmp);
	}

	//  Accessor / getter
	public int getDisks(){
		return this.disks;
	}

	//  Mutator / setter
	public void setDisks(int disks){
		this.disks = disks;
	}

	private static void move(int n, String fromPole, String toPole, String auxPole){
		if (n == 1) {
			System.out.println("Move the disk "+n+" from "+fromPole+" to "+toPole);
		}else{
			move(n-1, fromPole, auxPole, toPole);
			System.out.println("Move the disk "+n+" from "+fromPole+" to "+toPole);
			move(n-1, auxPole, toPole, fromPole);
		}
	}

	public void solve(){
		if (this.getDisks() >= 1) {
			move(this.getDisks(), "Pole1", "Pole3", "Pole2");
		}else{
			System.out.println("You should at least have 1 disk.");
		}
	}
}