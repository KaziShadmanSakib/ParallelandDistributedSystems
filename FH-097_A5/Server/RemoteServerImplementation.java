import java.io.BufferedReader;
import java.io.FileReader;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.io.IOException;

public class RemoteServerImplementation extends UnicastRemoteObject implements RemoteServerInterface {
    private static final String ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    public RemoteServerImplementation() throws RemoteException {
        
    }

    private String readCipherText() throws RemoteException{
        try (BufferedReader bufferedReader = new BufferedReader(new FileReader("Cipher.txt"))) {
            StringBuilder stringBuilder = new StringBuilder();
            String line;
            while ((line = bufferedReader.readLine()) != null) {
                stringBuilder.append(line);
            }
            return stringBuilder.toString();
        } catch (Exception e) {
            throw new RemoteException("Error reading cipher text file.", e);
        }
    }

    private char[][] readTabulaRecta() throws IOException {
        try (BufferedReader bufferdReader = new BufferedReader(new FileReader("TabulaRecta.txt"))) {
            char[][] tabulaRecta;
            tabulaRecta = new char[ALPHABET.length()][ALPHABET.length()];
            String line;

            for (int i = 0; i < ALPHABET.length(); i++) {
                line = bufferdReader.readLine();
                tabulaRecta[i] = line.toCharArray();
            }

            return tabulaRecta;
        }
    }

    private int findIndexInRow(char[] row, char target) {
        for (int i = 0; i < row.length; i++) {
            if (row[i] == target) {
                return i;
            }
        }
        return -1; // not found
    }

    @Override
    public String decryptCaesar(String keyword) throws RemoteException {
        try {
            String encryptedText = readCipherText();
    
            keyword = keyword.toUpperCase();
    
            StringBuilder decryptedText = new StringBuilder();
    
            char[] encryptedChars = encryptedText.toCharArray();
    
            for (int i = 0; i < encryptedChars.length; i++) {
                char c = encryptedChars[i];
    
                if (Character.isLetter(c)) {

                    int shift = keyword.charAt(i % keyword.length()) - 'A';
                    
                    int indexOfCurrentEncryptedLetter = ALPHABET.indexOf(c);
                    char decryptedChar = ALPHABET.charAt((indexOfCurrentEncryptedLetter - shift + 26) % 26);
    
                    decryptedText.append(decryptedChar);
                } else {
                    decryptedText.append(c);
                }
            }
    
            return decryptedText.toString();
    
        } catch (IOException e) {
            throw new RemoteException("Error decrypting with Caesar cipher. Failed to load Cipher text.", e);
        }
    }
    


    @Override
    public String decryptVigenere(String keyword) throws RemoteException {
    try {
        String encryptedText = readCipherText();  

        char[][] tabulaRecta = readTabulaRecta();

        keyword = keyword.toUpperCase();

        StringBuilder decryptedText = new StringBuilder();

        int keyLength = keyword.length();

        for (int i = 0; i < encryptedText.length(); i++) {
            char c = encryptedText.charAt(i);

            char keyChar = keyword.charAt(i % keyLength);

            if (Character.isUpperCase(c)) {
                int rowIndex = ALPHABET.indexOf(keyChar);

                int colIndex = findIndexInRow(tabulaRecta[rowIndex], c);

                char decryptedChar = ALPHABET.charAt(colIndex);

                decryptedText.append(decryptedChar);
            }
            else if (Character.isLowerCase(c)) {
                int rowIndex = ALPHABET.indexOf(Character.toUpperCase(keyChar));

                int colIndex = findIndexInRow(tabulaRecta[rowIndex], Character.toUpperCase(c));

                char decryptedChar = ALPHABET.charAt(colIndex);

                decryptedText.append(Character.toLowerCase(decryptedChar));
            }
            else {
                decryptedText.append(c);
            }
        }

        return decryptedText.toString();
        } catch (IOException e) {
            throw new RemoteException("Error decrypting with Vigenere cipher. Failed to load TabulaRecta.", e);
    } catch (Exception e) {
        throw new RemoteException("Error decrypting with Vigenere cipher.", e);
    }
    }
}