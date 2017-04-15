import java.io.File;
import java.util.ArrayList;
import java.util.Scanner;


//Alex Wilton and Steve Zelek

public class Main {

    public static void main(String[] args) {
        ArrayList<Item> inventory = loadInventory(args[0]);
        ArrayList<Item> basket = new ArrayList<>();

        Scanner scan = new Scanner(System.in);
        boolean isShopping = true;

        while (isShopping){
            displayActions();
            isShopping = performAction(scan, inventory, basket);
        }
    }

    public static ArrayList<Item> loadInventory (String filepath) {
        ArrayList<Item> inventory = new ArrayList<>();
        
    	try {
	        File file = new File(filepath);
	
	        //TODO: is it okay if the program errors out due to the inventory file being improper, and never starts?
            Scanner scan = new Scanner(file);

            while (scan.hasNext()) {
                double price = scan.nextDouble();
                String description = scan.nextLine().trim();

                //Ensures that only the first 40 characters of the description are taken.
                description = description.substring(0, Math.min(description.length(), 40));
                Item item = new Item(price, description);
                inventory.add(item);
            }

            scan.close();
        }
        //program errors out and returns an empty inventory if there was no file
        //if there was some data, may return an inventory with some data
        catch (Exception e){
            System.err.println(e);
        }

        return inventory;
    }

    public static boolean performAction(Scanner scan, ArrayList<Item> inventory, ArrayList<Item> basket){
        String action = scan.next().toLowerCase();

        boolean stillShopping = true;

        switch (action){
            case "a":
                try {
                    int id = scan.nextInt();
                    addItem(inventory, basket, id);
                }
                catch (Exception e){
                    System.out.println("Error: The item number you entered is not valid.");
                }
                break;
            case "b":
                printBasket(basket);
                break;
            case "i":
                printInventory(inventory);
                break;
            case "c":
                checkout(basket);
                stillShopping = false;
                break;
            default:
                System.out.println("Error: The action you entered is not valid.");
                break;
        }
        System.out.println();
        //flushes the rest of the input if there is anything else on the line
        scan.nextLine();

        return stillShopping;
    }

    public static void displayActions(){
        System.out.println("print (I)nventory, (A)dd item to basket, print (B)asket, (C)heckout-quit");
        System.out.print("Action choice? ");
    }

    public static void addItem(ArrayList<Item> inventory, ArrayList<Item> basket, int id){
        //TODO: any other error checking?
        try {
            basket.add(inventory.get(id));
        }
        //user error
        catch(Exception e){
            System.out.println("Error: The item number you entered is not valid.");
        }
    }

    public static void printList(ArrayList<Item> basket){
        for (int i = 0; i < basket.size(); ++i){
            System.out.printf("%-5d%-45s%5.2f%n", i, basket.get(i).getDescription(), basket.get(i).getPrice());
        }
    }

    public static void printInventory(ArrayList<Item> inventory){
        System.out.println("\n                  Item Description                Price");
        System.out.println("     -----------------------------------------   -------");
        printList(inventory);
    }

    public static void printBasket(ArrayList<Item> basket){
        System.out.println("\n                 Order Item Description           Price");
        System.out.println("     -----------------------------------------   -------");
        printList(basket);
    }

    public static void checkout(ArrayList<Item> basket){
        System.out.println("\nThank you for your order. Your order contains: ");
        printBasket(basket);

        System.out.printf("%nThere are %d items. Your total cost is $%.2f.", basket.size(), getTotalCost(basket));
    }

    public static double getTotalCost(ArrayList<Item> basket){
        double totalCost = 0;
        for (Item i : basket){
            totalCost += i.getPrice();
        }
        return totalCost;
    }
}