#include"Server.h"
int main() {
	while (true) {
		static Server one;
		one.start();
	}
}