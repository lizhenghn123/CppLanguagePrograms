#include <stdint.h>
#include <string>

class CRC32
{
public:
	CRC32();
	~CRC32();

	static uint32_t Digest(const std::string& src);
	static std::string HexDigest(const std::string& src);

public:
	void reInit();
	void Update(const std::string& src);
	uint32_t Final() const;
	// Finalizes the CRC operation and fills the buffer with the digest.
	//  Data is uint32_t_t
	void Final(void* digest) const;
	//  Hex encoding for result
	std::string HexFinal() const;

private:
	uint32_t result_;
};