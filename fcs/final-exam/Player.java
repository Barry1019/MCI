//==================================
// Foundations of Computer Science
// Student: Shaokang Ma
// id: a1790846
// Semester: 01
// Year: 2020
// Practical Exam Number: final
//===================================

public class Player extends Person{
	//  attributes
	private int id;
	private int numWins;
	private int numPlayed;
	public static int nextId = 1;
	
	//  Parametrized constructor
	public Player(String name, int age, int numWins, int numPlayed) {
		super(name, age);
		this.setNumWins(numWins);
		this.setNumPlayed(numPlayed);
		this.setId(nextId);
		nextId++;
	}
	
	//  Accessors / getters
	public int getId() {
		return id;
	}
	public int getNumWins() {
		return numWins;
	}
	public int getNumPlayed() {
		return numPlayed;
	}
	
	//  Mutators / setters
	public void setId(int id) {
		this.id = id;
	}
	public void setNumWins(int numWins) {
		this.numWins = numWins;
	}
	public void setNumPlayed(int numPlayed) {
		this.numPlayed = numPlayed;
	}
	
	//  increments the number of wins and the number of games played by this player
	public void win() {
		this.numWins++;
		this.numPlayed++;
	}
	
	//  increments the number of games played
	public void lose() {
		this.numPlayed++;
	}
	
	//  returns an integer value representing this player's ranking score
	public int getRanking() {
		if (this.getNumPlayed() == 0) {
			return 0;
		}else {
			int rank = this.getNumPlayed() * (this.getNumWins() / this.getNumPlayed());
			return rank;
		}
	}
	
	//  returns the String representing the player's full information
	@Override
	public String toString() {
		String info = super.toString();
		info = info + "Id: " + this.getId() + " Ranking: " + this.getRanking();
		return info;
	}
}
