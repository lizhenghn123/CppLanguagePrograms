用法：
	http://google.github.io/flatbuffers/md__cpp_usage.html
	http://google.github.io/flatbuffers/md__compiler.html
	
示例：
	flatc.exe -c person_infos.fbs  //生成person_infos_generated.h文件
	
说明：
	以person_infos.fbs为例
		namespace zl.persons;  
		  
		enum GENDER_TYPE : byte  
		{  
			MALE    = 0,  
			FEMALE  = 1,  
			OTHER   = 2  
		}  
		  
		table personal_info  
		{  
			id : uint;  
			name : string;  
			age : byte;  
			gender : GENDER_TYPE;  
			phone_num : ulong;  
		}  
		  
		table personal_info_list  
		{  
			info : [personal_info];  
		}  
		  
		root_type personal_info_list;

	生成的person_infos_generated.h中命名空间即是zl::persons;
	分别生成了枚举类GENDER_TYPE、结构体personal_info、结构体personal_info_list；另外还生成了结构体personal_infoBuilder和personal_info_listBuilder；
	还有一些相关的全局函数。

	注意上面的personal_info和personal_info_list是以table开头的，因为可以支持string类型，而struct不支持。