typedef struct head {
	unsigned char type;
	unsigned int len;
} head;

typedef struct data_structure {
	head *header;
	void *data;
} data_structure;

// - tip 1:
// 		''char*'' - numele celui_care_dedica (marime variabila);
// 		''int8_t'' prima bancnota;
// 		''int8_t'' a doua bancnota;
// 		''char*'' - numele celui_caruia i se dedica (marime variabila);
		
// - tip 2:
// 		''char*'' - numele celui_care_dedica (marime variabila);
// 		''int16_t'' prima bancnota;
// 		''int32_t'' a doua bancnota;
// 		''char*'' - numele celui_caruia i se dedica (marime variabila);
// - tip 3:

// 		''char*'' - numele celui_care_dedica (marime variabila);
// 		''int32_t'' prima bancnota;
// 		''int32_t'' a doua bancnota;
// 		''char*'' - numele celui_caruia i se dedica (marime variabila);

// printf("%"PRId8"\n", val)   //unde val este un int8_t
// printf("%"PRId16"\n", val)  //unde val este un int16_t
// printf("%"PRId32"\n", val)  //unde val este un int32_t