import java.util.LinkedList;

//==================================
// Foundations of Computer Science
// Student: Shaokang Ma
// id: a1790846
// Semester: 01
// Year: 2020
// Practical Exam Number: final
//===================================

public class Club {
	//  a collection to contain Players
	private LinkedList<Player> members = new LinkedList<>();
	
	//  adds the member to its collection of members
	public void addMember(Player member) {
		this.members.add(member);
	}
	
	//  remove a player with that id from its collection of members
	//  If the removal is successful this method should return true.
	//  If the player can't be found then this method should return false.
	public boolean removeMemberById(int id) {
		boolean isRemoved = false;
		for (int i = 0; i < this.members.size(); i++) {
			if (this.members.get(i).getId() == id) {
				this.members.remove(i);
				isRemoved = true;
				break;
			}
		}
		return isRemoved;
	}
	
	//  returns the Player in the club that has the highest ranking
	public Player getHighestRankedPlayer() {
		if (this.members.size() == 0) {
			return null;
		}else {
			//  if more than one player has the equal highest ranking
			//  the player who was added to the club most recently will be the one returned
			int rankHighest = -1;
			int indexOfHighestRank = this.members.size();
			for (int i = this.members.size() - 1; i >= 0; i--) {
				if (this.members.get(i).getRanking() > rankHighest) {
					rankHighest = this.members.get(i).getRanking();
					indexOfHighestRank = i;
				}
			}
			return this.members.get(indexOfHighestRank);
		}
	}
	
	//  print out all the members in the order in which they were added
	public void printMembers() {
		if (this.members.size() == 0) {
			return;
		}
		for (int i = 0; i < this.members.size(); i++) {
			System.out.println(this.members.get(i).toString());
		}
	}
}
