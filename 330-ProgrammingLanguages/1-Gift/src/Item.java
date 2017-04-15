public class Item {
    private String description;
    private double price;

    public Item(double price, String description) {
        this.price = price;
        this.description = description;
    }

    public String getDescription() {
        return description;
    }

    public double getPrice() {
        return price;
    }
}