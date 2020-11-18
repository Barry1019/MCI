public class WaitingList {
    protected Node head;
    protected int numNodes;

    public WaitingList(){
        this.head = null;
        this.numNodes = 0;
    }

    /**
     * This method will pop the first patient from the waiting list
     * @return the patient popped from the waiting list
     */
    public Patient popPatient() {
        if (this.head == null) {
            System.out.println("Error! The waiting list is empty.");
            return null;
        }else{
            Node tmp = this.head;
            this.head = this.head.getNext();
            return tmp.getPatient();
        }
    }

    /**
     * This method will add patient into the waiting list according to the triage level
     * @param patient patient's data
     */
    public void addToList(Patient patient) {
        if (this.head == null) {
            this.head = new Node(patient);
        }else{
            int triageLevel = patient.getTriageLevel();
            Node tmp = this.head;  //  set up a pointer
            while(tmp.getNext() != null && triageLevel <= tmp.getNext().getPatient().getTriageLevel()){
               tmp = tmp.getNext();     
            }
            Node p = new Node(patient);
            p.setNext(tmp.getNext());
            tmp.setNext(p);
        }
    }

    /**
     * print out the information for each patient in waiting list
     */
    public void printList() {
        if (this.head == null) {
            System.out.println("Error! The waiting list is empty.");
        }else{
            Node tmp = this.head;
            while(tmp != null){
                tmp.printNode();
                tmp = tmp.getNext();
            }
        }
    }

    /**
     * Check whether the patient is in this list or not
     * @return
     */
    public boolean isInList(Patient patient) {
        if (this.head == null) {
            return false;
        }
        Node temp = this.head;
        while(temp != null) {
            if(temp.getPatient().getName().equals(patient.getName())
                    && temp.getPatient().getPhoneNumber().equals(patient.getPhoneNumber())){
                return true;
            }
            temp = temp.getNext();
        }
        return false;
    }

    //  Accessor
    public Node getHead(){
        return this.head;
    }
}
