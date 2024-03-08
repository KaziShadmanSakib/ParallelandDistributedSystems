import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

public class RemoteServer {
    public static void main(String[] args) {
        try {
            RemoteServerImplementation remoteServer = new RemoteServerImplementation();

            Registry registry = LocateRegistry.createRegistry(10199);

            registry.rebind("RemoteServer", remoteServer);

            System.out.println("Remote Server Started Successfully.....");

        } catch (Exception e) {
            // Handle exceptions
            System.err.println("Server exception: " + e.toString());
            e.printStackTrace();
        }
    }
}
