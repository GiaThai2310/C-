#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Product{
	int product_id;
	char product_name[50];
	float price;
	int quantity;
};

void writeProducts(const char *filename, int numProducts);
void appendProducts(const char *filename, int numProducts);
void readProducts(const char *filename);
void modifyProduct(const char *filename);

int main(){
	const char *filename = "products.bin";
	int choice, numProducts;
	
	do{
		printf("\n-- Product Management System --\n");
		printf("1. Write Products\n");
		printf("2. Append Products\n");
		printf("3. Read Products\n");
		printf("4. Modify Products\n");
		printf("5. Exit\n");
		printf("Enter your choice: ");
		scanf("%d", &choice);
		switch (choice){
			case 1:
				printf("Enter the number of products to write: ");
				scanf("%d", &numProducts);
				writeProducts(filename, numProducts);
				break;
			case 2:
				printf("Enter the number of product to append: ");
				scanf("%d", &numProducts);
				appendProducts(filename, numProducts);
				break;
			case 3:
				readProducts(filename);
				break;
			case 4:
				modifyProduct(filename);
				break;
			case 5:
				printf("Exiting...\n");
				break;
			default:
				printf("Invalid choice. Please try again.\n");
		}
	}while (choice != 5);
	return 0;
}

//Write
void writeProducts(const char *filename, int numProducts){
	while(numProducts < 1){
		printf("Invalid number. Please enter again!\n");
		printf("Enter the number of products to write: ");
		scanf("%d", &numProducts);
	}
	
	FILE *f;
	f = fopen(filename, "wb");
	if(f == NULL){
	printf("\nFile could not be opened.");
	exit(1);
	}
	struct Product product;
	for(int i = 0; i < numProducts; i++){
		printf("\nEnter details for product %d:\n", i + 1);
		printf("Product ID: ");
		scanf("%d", &product.product_id);
		printf("Product Name: ");
		scanf("%49s", product.product_name);
		while (getchar() != '\n');
		printf("Price: ");
		scanf("%f", &product.price);
		printf("Quantity: ");
		scanf("%d", &product.quantity);
		fwrite(&product, sizeof(struct Product), 1, f);
	}
	puts("\nProducts have been written to the file successfully.");
	fclose(f);
}

//Append
void appendProducts(const char *filename, int numProducts){
	while(numProducts < 1){
		printf("Invalid number. Please enter again!\n");
		printf("Enter the number of products to write: ");
		scanf("%d", &numProducts);
	}
	FILE *f;
	f = fopen(filename, "ab+");
	if(f == NULL){
	printf("\nFile could not be opened.");
	exit(1);
	}
	struct Product product;
	int temp_id, isDuplicate = 0;
		
	for(int i = 0; i < numProducts; i++){
		printf("\nEnter details for product %d:\n", i + 1);
		do {
            isDuplicate = 0; // Reset flag for each new input
            rewind(f); // Move file pointer back to beginning

            
            printf("Product ID: ");
            scanf("%d", &temp_id);

            // Check if product ID already exists
            while (fread(&product, sizeof(struct Product), 1, f)){
                if (temp_id == product.product_id) {
                    printf("Product ID %d already exists. Please enter another product ID.\n", temp_id);
                    isDuplicate = 1;
                    break;
                }
            }
        }while (isDuplicate);
		product.product_id = temp_id;
		printf("Product Name: ");
		scanf("%49s", product.product_name);
		while (getchar() != '\n');
		printf("Price: ");
		scanf("%f", &product.price);
		printf("Quantity: ");
		scanf("%d", &product.quantity);
		fwrite(&product, sizeof(struct Product), 1, f);
	}
	puts("Products have been written to the file successfully.");
	fclose(f);
}

//Read
void readProducts(const char *filename){
	puts("\nReading products form the file:");
	puts("-----------------------------------------------------");
	puts("Product ID      Product Name        Price    Quantity");
	puts("-----------------------------------------------------");
	
	FILE *f;
	f = fopen(filename, "rb");
	if(f == NULL){
	printf("\nFile could not be opened.");
	exit(1);
	}
	struct Product product;  
    while(fread(&product, sizeof(struct Product), 1, f)) {
    
        printf("%-15d %-20s %-10.2f %d\n", 
               product.product_id, 
               product.product_name, 
               product.price, 
               product.quantity);
    }
	puts("-----------------------------------------------------\n");
	fclose(f);
}

//Modify
void modifyProduct(const char *filename){
		FILE *f;
	f = fopen(filename, "rb+");
	if(f == NULL){
	printf("\nFile could not be opened.");
	exit(1);
	}
	int id, found = 0;
	printf("Enter the Product ID to modify: ");
	scanf("%d", &id);
	struct Product product;
	
	while(fread(&product, sizeof(struct Product), 1, f)){
		if(product.product_id == id){
		found = 1;
		
		fseek(f, -(long)sizeof(struct Product), SEEK_CUR);
		printf("Enter new product name: ");
		scanf("%49s", product.product_name);
		while (getchar() != '\n');
		printf("Enter new price: ");
		scanf("%f", &product.price);
		printf("Enter new quantity: ");
		scanf("%d", &product.quantity);
		fwrite(&product, sizeof(struct Product), 1, f);
		break;
		}
	}
	if(!found){
		printf("Product ID %d could not be found.\n", id);
	}
	fclose(f);
}