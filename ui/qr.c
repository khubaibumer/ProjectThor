/*
 * qr.c
 *
 *  Created on: Jul 3, 2020
 *      Author: khubaibumer
 */

#include <thor.h>
#include <qrgen.h>

// Prints the given QR Code to the console.
static void print_qr(const uint8_t qrcode[], FILE *ui) {
	int size = qrcodegen_getSize(qrcode);
	int border = 4;
	for (int y = -border; y < size + border; y++) {
		for (int x = -border; x < size + border; x++) {
			fputs((qrcodegen_getModule(qrcode, x, y) ? "##" : "  "), ui);
		}
		fputs("\n", ui);
	}
	fputs("\n", ui);
}

int __send_qr_to_ui(void *ptr, FILE *ui, const char *information) {

	enum qrcodegen_Ecc errCorLvl = qrcodegen_Ecc_HIGH;  // Error correction level

	// Make and print the QR Code symbol
	uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
	uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
	bool ok = qrcodegen_encodeText(information, tempBuffer, qrcode, errCorLvl,
			qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO,
			true);
	if (ok)
		print_qr(qrcode, ui);

	return 0;
}
