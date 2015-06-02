#include "CRC32.h"
#include <sstream>

static const uint32_t* InitCrc32Table() {
	// CRC32 polynomial, in reversed form.
	static const uint32_t kCrc32Polynomial = 0xEDB88320;
	// See RFC 1952, or http://en.wikipedia.org/wiki/Cyclic_redundancy_check
	static uint32_t crc32_table[256];
	for (uint32_t i = 0; i < sizeof(crc32_table) / sizeof(crc32_table[0]); ++i) {
		uint32_t c = i;
		for (size_t j = 0; j < 8; ++j) {
			if (c & 1) {
				c = kCrc32Polynomial ^ (c >> 1);
			}
			else {
				c >>= 1;
			}
		}
		crc32_table[i] = c;
	}
	return crc32_table;
}

static const uint32_t* Crc32Table() {
	const uint32_t* table = InitCrc32Table();
	return table;
}



CRC32::CRC32() {
	reInit();
}

CRC32::~CRC32() {}

void CRC32::reInit() {
	result_ = 0U;
}

void CRC32::Update(const std::string& src) {
	uint32_t c = result_ ^ 0xFFFFFFFF;
	const uint8_t* u = reinterpret_cast<const uint8_t*>(src.data());
	const uint32_t* table = Crc32Table();
	for (size_t i = 0; i < src.size(); ++i) {
		c = table[(c ^ u[i]) & 0xFF] ^ (c >> 8);
	}
	result_ = c ^ 0xFFFFFFFF;
}

uint32_t CRC32::Final() const {
	return result_;
}

void CRC32::Final(void* data) const {
	memcpy(data, &result_, 4);
}

std::string CRC32::HexFinal() const {
	uint8_t digest[4];
	Final(&digest);
	std::ostringstream oss;
	oss << digest[0] << digest[1] << digest[2] << digest[3];
	return Hex::EncodeAsString(digest, 4);
	return oss.str();
}

uint32_t CRC32::Digest(const std::string& src) {
	CRC32 crc32;
	crc32.Update(src);
	return crc32.Final();
}

std::string CRC32::HexDigest(const std::string& src) {
	CRC32 crc32;
	crc32.Update(src);
	return crc32.HexFinal();
}
