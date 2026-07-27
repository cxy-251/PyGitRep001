classDiagram
    %% Factory Method
    class Product
    class Creator {
        +createProduct() Product
    }
    class ConcreteProduct
    class ConcreteCreator
    Creator <|-- ConcreteCreator
    Product <|-- ConcreteProduct
    ConcreteCreator --> Product : create()