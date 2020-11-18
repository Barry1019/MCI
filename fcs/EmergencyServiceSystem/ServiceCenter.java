public class ServiceCenter {
    private WaitingList awl;

    public ServiceCenter() {
        this.awl = new WaitingList();
    }

    /**
     * Record patient's data and add the patient into waiting list
     * @param name
     * @param phoneNumber
     * @param triageLevel
     * @param location
     */
    public void addPatientIntoList(String name, String phoneNumber, int triageLevel, String location) {
        Patient patient = new Patient(name, phoneNumber, triageLevel, location);
        if(this.awl.isInList(patient)){
            System.out.println(patient.getName() + " is in waiting list. ");
        }else{
            this.awl.addToList(patient);
            System.out.println("Add " + patient.getName() + " into waiting list. ");
        }
    }

    /**
     * Pop out the first patient in the waiting list and assign an Ambulance for him/her
     * @return the patient object
     */
    public Patient assignAmbulanceForPatient() {
        if (this.awl.getHead() == null) {
            System.out.println(" Error! The waiting list is empty.");
            return null;
        }else{
            Patient patient = this.awl.popPatient();
            System.out.println("Assigned an ambulance for patient: " + patient.getName());
            return patient;
        }
    }

    //  pop out one patient by his/her id
    public Patient assignAmbulanceForPatientById(int id){
        if (this.awl.getHead() == null) {
            System.out.println(" Error! The waiting list is empty.");
            return null;
        }else if (awl.getHead().getPatient().getId() == id) {
            Patient patient = this.awl.popPatient();
            System.out.println("Success! An ambulance as assigned for patient " + patient.getId()+".");
            return patient;
        }

        Node prev = null;
        Node tmp = this.awl.getHead();
        while(tmp != null && tmp.getPatient().getId() != id){
            prev = tmp;
            tmp = tmp.getNext();
        }

        if (tmp == null) {
            System.out.println("The id is not in the list.");
            return null;
        }else{
            Patient patient = tmp.getPatient();
            prev.setNext(tmp.getNext());
            System.out.println("Success! An ambulance as assigned for patient " + patient.getId()+".");
            return patient;
        }
    }

    //  check how many patients are waiting before this patient given the patient's Id
    public void checkPositionById(int id){
        if (this.awl.getHead() == null) {
            System.out.println(" Error! The waiting list is empty.");
            return;
        }else if (this.awl.getHead().getPatient().getId() == id) {
            System.out.println("There is 0 patient before patient " + id +".");
        }else{
            int count = 0;
            Node tmp = this.awl.getHead();

            while(tmp != null && tmp.getPatient().getId() != id){
                tmp = tmp.getNext();
                count++;
            }

            if (tmp == null) {
                System.out.println("The id is not in the list.");
            }else{
            System.out.println("There are " + count + " patients before patient "+ id +".");
            }
        }
    }

    /**
     * Print out the waiting list
     */
    public void printWaitingList(){
        this.awl.printList();
    }

    public WaitingList getWaitingList(){
        return this.awl;
    }
}
