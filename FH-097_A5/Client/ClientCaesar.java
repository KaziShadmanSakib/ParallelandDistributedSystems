import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

public class ClientCaesar {
    public static void main(String[] args) {
        try {
            Registry registry = LocateRegistry.getRegistry("localhost", 10199);
            RemoteServerInterface remoteServer = (RemoteServerInterface) registry.lookup("RemoteServer");

            String keyword = "GORDIAN"; 
            String decryptedMessage = remoteServer.decryptCaesar(keyword);

            System.out.println("Decrypted Message (Caesar Cipher):\n"+ decryptedMessage);
        } catch (Exception e) {
            System.err.println("Client exception: " + e.toString());
            e.printStackTrace();
        }
    }
}
