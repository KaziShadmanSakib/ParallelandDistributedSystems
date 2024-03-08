import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

public class ClientVigenere {
    public static void main(String[] args) {
        try {
            Registry registry = LocateRegistry.getRegistry("localhost", 10199);

            RemoteServerInterface remoteServer = (RemoteServerInterface) registry.lookup("RemoteServer");

            String keyword = "GORDIAN";  // Replace with the actual Vigenere cipher keyword

            String decryptedMessage = remoteServer.decryptVigenere(keyword);

            System.out.println("Decrypted Message (Vigenere Cipher):\n" + decryptedMessage);
            
        } catch (Exception e) {
            System.err.println("Client exception: " + e.toString());
            e.printStackTrace();
        }
    }
}
