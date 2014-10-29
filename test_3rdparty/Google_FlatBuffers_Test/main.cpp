#include <iostream>
#include <random>
#include <cassert>
#include "flatbuffers/flatbuffers.h"
#include "flatbuffers/idl.h"
#include "flatbuffers/util.h"
#include "monster_generated.h"       // 参考自 google flatbuffers demo
#include "person_infos_generated.h"  // 参考自 http://blog.csdn.net/menggucaoyuan/article/details/34409433
#include "base/StopWatch.h"

namespace fb = flatbuffers;
#define fb_offset                 fb::Offset
#define fb_string                 fb::String
#define fb_vector                 fb::Vector
#define fb_table                  fb::Table
#define fb_builder                fb::FlatBufferBuilder
#define fb_create_string(b, ...)  (b).CreateString(__VA_ARGS__)
#define fb_create_vector(b, ...)  (b).CreateVector(__VA_ARGS__)
#define fb_vector_size(v)         (unsigned)(*(v)).Length()
#define fb_vector_length(v)       (unsigned)(*(v)).Length()
#define fb_vector_at(v, i)        (*(v)).Get(i)
#define fb_get_buf(b)             (b).GetBufferPointer()
#define fb_get_size(b)            (unsigned)(b).GetSize()
#define fb_clear(b)               (b).Clear()
#define fb_finish(b, buf)         (b).Finish(buf)

// example of how to build up a serialized buffer algorithmically:
std::string CreateFlatBufferTest()
{
	using namespace MyGame::Example;

	flatbuffers::FlatBufferBuilder builder;

	auto vec = Vec3(1, 2, 3, 0, 0, Test(10, 20));

	auto name = builder.CreateString("MyMonster");

	unsigned char inv_data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	auto inventory = builder.CreateVector(inv_data, 10);

	Test tests[] = { Test(10, 20), Test(30, 40) };
	auto testv = builder.CreateVectorOfStructs(tests, 2);

	// create monster with very few fields set:
	// (same functionality as CreateMonster below, but sets fields manually)
	MonsterBuilder mb(builder);
	mb.add_hp(20);
	auto mloc2 = mb.Finish();

	// Create an array of strings:
	flatbuffers::Offset<flatbuffers::String> strings[2];
	strings[0] = builder.CreateString("bob");
	strings[1] = builder.CreateString("fred");
	auto vecofstrings = builder.CreateVector(strings, 2);

	// Create an array of tables:
	auto vecoftables = builder.CreateVector(&mloc2, 1);

	// shortcut for creating monster with all fields set:
	auto mloc = CreateMonster(builder, &vec, 150, 80, name, inventory, Color_Blue,
		Any_Monster, mloc2.Union(), // Store a union.
		testv, vecofstrings, vecoftables, 0);

	builder.Finish(mloc);

#ifdef FLATBUFFERS_TEST_VERBOSE
	// print byte data for debugging:
	auto p = builder.GetBufferPointer();
	for (flatbuffers::uoffset_t i = 0; i < builder.GetSize(); i++)
		printf("%d ", p[i]);
#endif

	// return the buffer for the caller to use.
	return std::string(reinterpret_cast<const char *>(builder.GetBufferPointer()),
		builder.GetSize());
}


#define TEST_OUTPUT_LINE(...) { printf(__VA_ARGS__); printf("\n"); }

template<typename T, typename U>
void TestEq(T expval, U val, const char *exp, const char *file, int line) {
	if (expval != val) {
		auto expval_str = flatbuffers::NumToString(expval);
		auto val_str = flatbuffers::NumToString(val);
		TEST_OUTPUT_LINE("TEST FAILED: %s:%d, %s (%s) != %s", file, line,
			exp, expval_str.c_str(), val_str.c_str());
		assert(0);
	}
}

#define TEST_EQ(exp, val) TestEq(exp,         val,   #exp, __FILE__, __LINE__)
#define TEST_NOTNULL(exp) TestEq(exp == NULL, false, #exp, __FILE__, __LINE__)

//  example of accessing a buffer loaded in memory:
void AccessFlatBufferTest(const std::string &flatbuf)
{
	using namespace MyGame::Example;

	// First, verify the buffers integrity (optional)
	flatbuffers::Verifier verifier(
		reinterpret_cast<const uint8_t *>(flatbuf.c_str()),
		flatbuf.length());
	TEST_EQ(VerifyMonsterBuffer(verifier), true);

	// Access the buffer from the root.
	auto monster = GetMonster(flatbuf.c_str());

	TEST_EQ(monster->hp(), 80);
	TEST_EQ(monster->mana(), 150);  // default
	TEST_EQ(strcmp(monster->name()->c_str(), "MyMonster"), 0);
	// Can't access the following field, it is deprecated in the schema,
	// which means accessors are not generated:
	// monster.friendly()

	auto pos = monster->pos();
	TEST_NOTNULL(pos);
	TEST_EQ(pos->z(), 3);
	TEST_EQ(pos->test3().a(), 10);
	TEST_EQ(pos->test3().b(), 20);

	auto inventory = monster->inventory();
	TEST_NOTNULL(inventory);
	unsigned char inv_data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	for (flatbuffers::uoffset_t i = 0; i < inventory->Length(); i++)
		TEST_EQ(inventory->Get(i), inv_data[i]);

	// Example of accessing a union:
	TEST_EQ(monster->test_type(), Any_Monster);  // First make sure which it is.
	auto monster2 = reinterpret_cast<const Monster *>(monster->test());
	TEST_NOTNULL(monster2);
	TEST_EQ(monster2->hp(), 20);

	// Example of accessing a vector of strings:
	auto vecofstrings = monster->testarrayofstring();
	TEST_EQ(vecofstrings->Length(), 2U);
	TEST_EQ(strcmp(vecofstrings->Get(0)->c_str(), "bob"), 0);
	TEST_EQ(strcmp(vecofstrings->Get(1)->c_str(), "fred"), 0);

	// Example of accessing a vector of tables:
	auto vecoftables = monster->testarrayoftables();
	TEST_EQ(vecoftables->Length(), 1U);
	TEST_EQ(vecoftables->Get(0)->hp(), 20);

	// Since Flatbuffers uses explicit mechanisms to override the default
	// compiler alignment, double check that the compiler indeed obeys them:
	// (Test consists of a short and byte):
	TEST_EQ(flatbuffers::AlignOf<Test>(), 2UL);
	TEST_EQ(sizeof(Test), 4UL);

	auto tests = monster->test4();
	TEST_NOTNULL(tests);
	auto &test_0 = tests->Get(0);
	auto &test_1 = tests->Get(1);
	TEST_EQ(test_0.a(), 10);
	TEST_EQ(test_0.b(), 20);
	TEST_EQ(test_1.a(), 30);
	TEST_EQ(test_1.b(), 40);
}

inline flatbuffers::Offset<zl::persons::personal_info> create_personal_info(
	flatbuffers::FlatBufferBuilder &_fbb,
	uint32_t id,
	flatbuffers::Offset<flatbuffers::String> name,
	int8_t age,
	int8_t gender,
	uint64_t phone_num)
{
	zl::persons::personal_infoBuilder builder_(_fbb);
	builder_.add_phone_num(phone_num);
	builder_.add_name(name);
	builder_.add_id(id);
	builder_.add_gender(gender);
	builder_.add_age(age);
	return builder_.Finish();
}

inline fb_offset<zl::persons::personal_info_list> create_personal_info_list(fb_builder &_fbb,
	fb_offset<fb_vector<fb_offset<zl::persons::personal_info>>> info)
{
	zl::persons::personal_info_listBuilder builder_(_fbb);
	builder_.add_info(info);
	return builder_.Finish();
}

inline const zl::persons::personal_info_list *get_personal_info_list(const void *buf)
{
	return fb::GetRoot<zl::persons::personal_info_list>(buf);
}

#define ARRAY_SIZE 37

std::string encode_persons()
{
	fb_builder builder;
	unsigned id;
	int idx = 0;
	char name[32];
	int age;
	int gender;
	unsigned long long phone;
	fb_offset<fb_string> fb_name;
	fb_offset<zl::persons::personal_info> info[ARRAY_SIZE];
	fb_offset<fb_vector<fb_offset<zl::persons::personal_info>>> info_array;
	fb_offset<zl::persons::personal_info_list> info_list;
	int data_buf_len;

	for (idx = 0; idx < ARRAY_SIZE; idx++) {
		id = idx;
		sprintf(name, "gfb %u", id);
		fb_name = fb_create_string(builder, name);
		age = 13 + idx;
		gender = id % 3;
		phone = 1234567890 + id;
		info[idx] = create_personal_info(
			builder,
			id,
			fb_name,
			age,
			gender,
			phone);
	}

	info_array = fb_create_vector(builder, info, sizeof(info) / sizeof(info[0]));
	info_list = create_personal_info_list(builder, info_array);
	fb_finish(builder, info_list);

	// return the buffer for the caller to use.
	return std::string(reinterpret_cast<const char *>(builder.GetBufferPointer()),
		builder.GetSize());
}

void decode_persons(const char* data_buf, int len)
{
	// First, verify the buffers integrity (optional)
	flatbuffers::Verifier verifier(	reinterpret_cast<const uint8_t *>(data_buf), len);
	TEST_EQ(zl::persons::Verifypersonal_info_listBuffer(verifier), true);

	int idx = 0;
	const zl::persons::personal_info* info_ptr = NULL;
	unsigned info_vector_size = 0;
	const fb_vector<fb_offset<zl::persons::personal_info>>* info_vector = NULL;
	const zl::persons::personal_info_list* info_list_obj = NULL;

	if (!data_buf || !len)
	{
		return;
	}

	printf("buf size:%u\n", len);
	info_list_obj = get_personal_info_list(data_buf);
	info_vector = info_list_obj->info();
	//personal_info* info = fb_vector_at(info_vector, 0);
	info_vector_size = fb_vector_size(info_vector);
	assert(ARRAY_SIZE == info_vector_size);
	printf("info vecotor size:%u, its right size:%u\n", info_vector_size, ARRAY_SIZE);
	for (idx = 0; idx < info_vector_size; idx++) 
	{
		info_ptr = fb_vector_at(info_vector, idx);
		printf("info{id = %d, name = %s, age = %d, gender = %s, phone = %llu}\n",
			info_ptr->id(),
			info_ptr->name()->c_str(),
			info_ptr->age(),
			zl::persons::EnumNameGENDER_TYPE(info_ptr->gender()),
			info_ptr->phone_num());
	}
}

std::string CreateOnePerson()
{
	flatbuffers::FlatBufferBuilder builder;
	fb_offset<fb_string> name = builder.CreateString("hello word");

	zl::persons::personal_infoBuilder pib(builder);
	pib.add_id(1);
	pib.add_age(25);
	pib.add_gender(zl::persons::GENDER_TYPE_MALE);
	pib.add_name(name);
	pib.add_phone_num(1234567890);
	flatbuffers::Offset<zl::persons::personal_info> personinfo = pib.Finish();

	fb_offset<zl::persons::personal_info> info[1];
	info[0] = personinfo;

	fb_offset<fb_vector<fb_offset<zl::persons::personal_info>>> info_array = fb_create_vector(builder, info, sizeof(info) / sizeof(info[0]));
	fb_offset<zl::persons::personal_info_list> info_list = create_personal_info_list(builder, info_array);
	fb_finish(builder, info_list);

	// return the buffer for the caller to use.
	return std::string(reinterpret_cast<const char *>(builder.GetBufferPointer()), builder.GetSize());
}

void ParseOnePerson(const char* data_buf, int len)
{
	// First, verify the buffers integrity (optional)
	flatbuffers::Verifier verifier(	reinterpret_cast<const uint8_t *>(data_buf), len);
	TEST_EQ(zl::persons::Verifypersonal_info_listBuffer(verifier), true);

	int idx = 0;
	const zl::persons::personal_info* info_ptr = NULL;
	unsigned info_vector_size = 0;
	const fb_vector<fb_offset<zl::persons::personal_info>>* info_vector = NULL;
	const zl::persons::personal_info_list* info_list_obj = NULL;

	if (!data_buf || !len)
	{
		return;
	}

	printf("buf size:%u\n", len);
	info_list_obj = get_personal_info_list(data_buf);
	info_vector = info_list_obj->info();
	//personal_info* info = fb_vector_at(info_vector, 0);
	info_vector_size = fb_vector_size(info_vector);

	for (idx = 0; idx < info_vector_size; idx++) 
	{
		info_ptr = fb_vector_at(info_vector, idx);
		printf("info{id = %d, name = %s, age = %d, gender = %s, phone = %llu}\n",
			info_ptr->id(),
			info_ptr->name()->c_str(),
			info_ptr->age(),
			zl::persons::EnumNameGENDER_TYPE(info_ptr->gender()),
			info_ptr->phone_num());
	}
}

void Test_FlatBuffer_Performance()
{
	const int LOOP = 1000000; //1000000;
	
	std::string flatbufs;
	flatbufs.reserve(10000);

	zl::StopWatch watch;
	for (int idx = 0; idx < LOOP; idx++) 
	{
		flatbufs.clear();
		flatbufs = encode_persons();
	}

	double time = watch.elapsedTimeInMill();
	printf("loop = %d, time diff = %lf ms, or %lf s\n", LOOP, time, time/1000);
}

int main()
{
	{
		std::string flatbuf = encode_persons();
		printf("data size[%d]\n",flatbuf.size());

		decode_persons(flatbuf.c_str(), flatbuf.size());
		printf("---------------------OK---------------------\n");
	}
	{
		auto flatbuf = CreateOnePerson();
		ParseOnePerson(flatbuf.c_str(), flatbuf.size());
		printf("---------------------OK---------------------\n");
	}

	Test_FlatBuffer_Performance();

	{
		auto flatbuf = CreateFlatBufferTest();
		AccessFlatBufferTest(flatbuf);
		//std::cout << flatbuf<<"\n";
		printf("---------------------OK---------------------\n");
	}

	system("pause");
	return 0;
}