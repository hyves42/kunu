/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */

#include "tests_events_dispatch.h"
#include "kn_events_dispatch.h"
#include <check.h>
#include <stdio.h>
#include <stdbool.h>


static bool handler1_called=false;
static bool handler2_called=false;
static bool handler3_called=false;
static bool handler4_called=false;
static bool handler5_called=false;
static int handler1_count=0;
static int handler2_count=0;
static int handler3_count=0;
static int handler4_count=0;
static int handler5_count=0;

static void reset_states(void){
	handler1_called=false;
	handler2_called=false;
	handler3_called=false;
	handler4_called=false;
	handler5_called=false;

	handler1_count=0;
	handler2_count=0;
	handler3_count=0;
	handler4_count=0;
	handler5_count=0;
}

static void event_handler1(kn_event_t *event, void *user_data){
	ck_assert(event->id==1);
	handler1_called=true;
	handler1_count++;
}
static void event_handler2(kn_event_t *event, void *user_data){
	ck_assert(event->id==2);
	handler2_called=true;
	handler2_count++;
}
static void event_handler3(kn_event_t *event, void *user_data){
	ck_assert(event->id==3);
	handler3_called=true;
	handler3_count++;
}
static void event_handler4(kn_event_t *event, void *user_data){
	ck_assert(event->id==4);
	handler4_called=true;
	handler4_count++;
}
static void event_handler5(kn_event_t *event, void *user_data){
	ck_assert(event->id==5);
	handler5_called=true;
	handler5_count++;
}

static void dummy_handler(kn_event_t *event, void *user_data){
}

static kn_event_dispatcher_t disp={};

START_TEST(events_tests_init)
{
	kn_event_reg_t map[10]={};
	int map_count=10;
	int ret=kn_eventdisp_init(&disp, map, map_count);

	ck_assert(ret==0);

	ck_assert(disp.worker.on_event==kn_eventdisp_on_event);
	ck_assert(disp.worker.user_data==&disp);
}
END_TEST

START_TEST(events_tests_init2)
{
	kn_event_reg_t map[10]={};
	int map_count=10;

	kn_event_reg_t reg={
		.id=1,
		.user_data=NULL,
		.handler=dummy_handler
	};

	int ret=kn_eventdisp_init(&disp, map, map_count);
	ck_assert(ret==0);

	ret=kn_eventdisp_subscribe_reg(&disp, &reg);
	ck_assert(ret==0);
	ck_assert(kn_eventdisp_get_registered_count(&disp)==1);

	ck_assert(map[0].id==reg.id);
	ck_assert(map[0].user_data==reg.user_data);
	ck_assert(map[0].handler==reg.handler);
}
END_TEST

START_TEST(events_tests_init3)
{
	kn_event_reg_t map[2]={};
	int map_count=2;

	kn_event_reg_t reg1={
		.id=1,
		.user_data=NULL,
		.handler=dummy_handler
	};
	kn_event_reg_t reg2={
		.id=2,
		.user_data=NULL,
		.handler=dummy_handler
	};

	int ret=kn_eventdisp_init(&disp, map, map_count);
	ck_assert(ret==0);

	ret=kn_eventdisp_subscribe_reg(&disp, &reg1);
	ck_assert(ret==0);
	ck_assert(kn_eventdisp_get_registered_count(&disp)==1);

	ret=kn_eventdisp_subscribe_reg(&disp, &reg2);
	ck_assert(ret==0);
	ck_assert(kn_eventdisp_get_registered_count(&disp)==2);	

	ck_assert(map[0].id==reg1.id);
	ck_assert(map[0].user_data==reg1.user_data);
	ck_assert(map[0].handler==reg1.handler);


	ck_assert(map[1].id==reg2.id);
	ck_assert(map[1].user_data==reg2.user_data);
	ck_assert(map[1].handler==reg2.handler);	
}
END_TEST

//Check that we can't register more events than the size of the registered events array
START_TEST(events_tests_init4)
{
	kn_event_reg_t map[1]={};
	int map_count=1;

	kn_event_reg_t reg={
		.id=1,
		.user_data=NULL,
		.handler=dummy_handler
	};

	int ret=kn_eventdisp_init(&disp, map, map_count);
	ck_assert(ret==0);

	ret=kn_eventdisp_subscribe_reg(&disp, &reg);
	ck_assert(ret==0);
	ck_assert(kn_eventdisp_get_registered_count(&disp)==1);

	ret=kn_eventdisp_subscribe_reg(&disp, &reg);
	ck_assert(ret==-1);
	ck_assert(kn_eventdisp_get_registered_count(&disp)==1);	
}
END_TEST

//Check ordering
START_TEST(events_tests_init5)
{
	kn_event_reg_t map[6]={};
	int map_count=6;

	kn_event_reg_t reg1={
		.id=1,
		.user_data=NULL,
		.handler=dummy_handler
	};
	kn_event_reg_t reg2={
		.id=2,
		.user_data=NULL,
		.handler=dummy_handler
	};
	kn_event_reg_t reg3={
		.id=3,
		.user_data=NULL,
		.handler=dummy_handler
	};
	kn_event_reg_t reg4={
		.id=4,
		.user_data=NULL,
		.handler=dummy_handler
	};
	kn_event_reg_t reg5={
		.id=5,
		.user_data=NULL,
		.handler=dummy_handler
	};

	int ret=kn_eventdisp_init(&disp, map, map_count);
	ck_assert(ret==0);

	ret=kn_eventdisp_subscribe_reg(&disp, &reg1);
	ck_assert(ret==0);
	ck_assert(kn_eventdisp_get_registered_count(&disp)==1);

	ret=kn_eventdisp_subscribe_reg(&disp, &reg5);
	ck_assert(ret==0);
	ck_assert(kn_eventdisp_get_registered_count(&disp)==2);

	ret=kn_eventdisp_subscribe_reg(&disp, &reg2);
	ck_assert(ret==0);
	ck_assert(kn_eventdisp_get_registered_count(&disp)==3);	

	ret=kn_eventdisp_subscribe_reg(&disp, &reg4);
	ck_assert(ret==0);
	ck_assert(kn_eventdisp_get_registered_count(&disp)==4);

	ret=kn_eventdisp_subscribe_reg(&disp, &reg3);
	ck_assert(ret==0);
	ck_assert(kn_eventdisp_get_registered_count(&disp)==5);


	ck_assert(map[0].id==reg1.id);
	ck_assert(map[0].user_data==reg1.user_data);
	ck_assert(map[0].handler==reg1.handler);

	ck_assert(map[1].id==reg2.id);
	ck_assert(map[1].user_data==reg2.user_data);
	ck_assert(map[1].handler==reg2.handler);

	ck_assert(map[2].id==reg3.id);
	ck_assert(map[2].user_data==reg3.user_data);
	ck_assert(map[2].handler==reg3.handler);

	ck_assert(map[3].id==reg4.id);
	ck_assert(map[3].user_data==reg4.user_data);
	ck_assert(map[3].handler==reg4.handler);

	ck_assert(map[4].id==reg5.id);
	ck_assert(map[4].user_data==reg5.user_data);
	ck_assert(map[4].handler==reg5.handler);
}
END_TEST



//Check ordering
START_TEST(events_tests_init6)
{
	kn_event_reg_t map[6]={};
	int map_count=6;

	kn_event_reg_t reg1={
		.id=1,
		.user_data=NULL,
		.handler=dummy_handler
	};
	kn_event_reg_t reg2={
		.id=2,
		.user_data=NULL,
		.handler=dummy_handler
	};
	kn_event_reg_t reg3={
		.id=3,
		.user_data=NULL,
		.handler=dummy_handler
	};
	kn_event_reg_t reg4={
		.id=3,
		.user_data=NULL,
		.handler=dummy_handler
	};
	kn_event_reg_t reg5={
		.id=5,
		.user_data=NULL,
		.handler=dummy_handler
	};

	int ret=kn_eventdisp_init(&disp, map, map_count);
	ck_assert(ret==0);

	ret=kn_eventdisp_subscribe_reg(&disp, &reg1);
	ck_assert(ret==0);
	ck_assert(kn_eventdisp_get_registered_count(&disp)==1);

	ret=kn_eventdisp_subscribe_reg(&disp, &reg5);
	ck_assert(ret==0);
	ck_assert(kn_eventdisp_get_registered_count(&disp)==2);

	ret=kn_eventdisp_subscribe_reg(&disp, &reg2);
	ck_assert(ret==0);
	ck_assert(kn_eventdisp_get_registered_count(&disp)==3);	

	ret=kn_eventdisp_subscribe_reg(&disp, &reg4);
	ck_assert(ret==0);
	ck_assert(kn_eventdisp_get_registered_count(&disp)==4);

	ret=kn_eventdisp_subscribe_reg(&disp, &reg3);
	ck_assert(ret==0);
	ck_assert(kn_eventdisp_get_registered_count(&disp)==5);

	ck_assert(map[0].id==reg1.id);

	ck_assert(map[1].id==reg2.id);

	ck_assert((map[2].id==reg3.id) || (map[2].id==reg4.id));

	ck_assert((map[3].id==reg3.id) || (map[3].id==reg4.id));

	ck_assert(map[4].id==reg5.id);
}
END_TEST


START_TEST(events_tests_array)
{
	kn_event_reg_t map[6]={};
	int map_count=6;

	kn_event_reg_t array[]={{
		.id=1,
		.user_data=NULL,
		.handler=dummy_handler
	},{
		.id=2,
		.user_data=NULL,
		.handler=dummy_handler
	},{
		.id=3,
		.user_data=NULL,
		.handler=dummy_handler
	},{
		.id=4,
		.user_data=NULL,
		.handler=dummy_handler
	},{
		.id=5,
		.user_data=NULL,
		.handler=dummy_handler
	}};

	int ret=kn_eventdisp_init(&disp, map, map_count);
	ck_assert(ret==0);

	ret=kn_eventdisp_subscribe_reg_array(&disp, array, sizeof(array)/sizeof(array[0]));
	ck_assert(ret==0);
	ck_assert(kn_eventdisp_get_registered_count(&disp)==5);

	ck_assert(map[0].id==array[0].id);
	ck_assert(map[1].id==array[1].id);
	ck_assert(map[2].id==array[2].id);
	ck_assert(map[3].id==array[3].id);
	ck_assert(map[4].id==array[4].id);
	
}
END_TEST




//Test failure when we try to register too big an array
START_TEST(events_tests_array2)
{
	kn_event_reg_t map[3]={};
	int map_count=3;

	kn_event_reg_t array[]={{
		.id=1,
		.user_data=NULL,
		.handler=dummy_handler
	},{
		.id=2,
		.user_data=NULL,
		.handler=dummy_handler
	},{
		.id=3,
		.user_data=NULL,
		.handler=dummy_handler
	},{
		.id=4,
		.user_data=NULL,
		.handler=dummy_handler
	},{
		.id=5,
		.user_data=NULL,
		.handler=dummy_handler
	}};

	int ret=kn_eventdisp_init(&disp, map, map_count);
	ck_assert(ret==0);

	ret=kn_eventdisp_subscribe_reg_array(&disp, array, sizeof(array)/sizeof(array[0]));
	ck_assert(ret<0);
	ck_assert(kn_eventdisp_get_registered_count(&disp)==3);	
}
END_TEST


START_TEST(events_tests_static_init)
{

	kn_event_reg_t array[]={{
		.id=1,
		.user_data=NULL,
		.handler=dummy_handler
	},{
		.id=2,
		.user_data=NULL,
		.handler=dummy_handler
	},{
		.id=3,
		.user_data=NULL,
		.handler=dummy_handler
	},{
		.id=4,
		.user_data=NULL,
		.handler=dummy_handler
	},{
		.id=5,
		.user_data=NULL,
		.handler=dummy_handler
	}};

	kn_event_dispatcher_t disp=KN_EVENTDISP_INIT_WITH_FIXED_MAP(disp, array);

	ck_assert(kn_eventdisp_get_registered_count(&disp)==5);

	ck_assert(disp.map==array);
	ck_assert(disp.worker.on_event==kn_eventdisp_on_event);
	ck_assert(disp.worker.user_data==&disp);
	
}
END_TEST


//Check broadcast of 1 event
START_TEST(events_tests_broadcast)
{
	kn_event_reg_t map[6]={};
	int map_count=6;
	bool handler_called=false;

	void handler3(kn_event_t *event, void *user_data){
		ck_assert(event->id==3);
		handler_called=true;
	}


	kn_event_reg_t array[]={{
		.id=1,
		.user_data=NULL,
		.handler=dummy_handler
	},{
		.id=2,
		.user_data=NULL,
		.handler=dummy_handler
	},{
		.id=3,
		.user_data=NULL,
		.handler=handler3
	},{
		.id=4,
		.user_data=NULL,
		.handler=dummy_handler
	},{
		.id=5,
		.user_data=NULL,
		.handler=dummy_handler
	}};


	kn_event_t evt={
		.id=3
	};
	
	int ret=kn_eventdisp_init(&disp, map, map_count);
	ck_assert(ret==0);

	ret=kn_eventdisp_subscribe_reg_array(&disp, array, sizeof(array)/sizeof(array[0]));
	ck_assert(ret==0);
	ck_assert(kn_eventdisp_get_registered_count(&disp)==5);

	ret=kn_eventdisp_publish(&disp, &evt);
	ck_assert(ret==0);
	ck_assert(handler_called);
}
END_TEST





START_TEST(events_tests_broadcast2)
{
	kn_event_reg_t map[6]={};
	int map_count=6;


	kn_event_reg_t array[]={{
		.id=1,
		.user_data=NULL,
		.handler=event_handler1
	},{
		.id=2,
		.user_data=NULL,
		.handler=event_handler2
	},{
		.id=3,
		.user_data=NULL,
		.handler=event_handler3
	},{
		.id=4,
		.user_data=NULL,
		.handler=event_handler4
	},{
		.id=5,
		.user_data=NULL,
		.handler=event_handler5
	}};

	kn_event_t evt={};

	int ret=kn_eventdisp_init(&disp, map, map_count);
	ck_assert(ret==0);

	reset_states();

	ret=kn_eventdisp_subscribe_reg_array(&disp, array, sizeof(array)/sizeof(array[0]));
	ck_assert(ret==0);
	ck_assert(kn_eventdisp_get_registered_count(&disp)==5);

	evt.id=1;
	ret=kn_eventdisp_publish(&disp, &evt);
	ck_assert(ret==0);
	evt.id=2;
	ret=kn_eventdisp_publish(&disp, &evt);
	ck_assert(ret==0);
	evt.id=3;
	ret=kn_eventdisp_publish(&disp, &evt);
	ck_assert(ret==0);
	evt.id=4;
	ret=kn_eventdisp_publish(&disp, &evt);
	ck_assert(ret==0);
	evt.id=5;
	ret=kn_eventdisp_publish(&disp, &evt);
	ck_assert(ret==0);

	ck_assert(handler1_count==1);
	ck_assert(handler2_count==1);
	ck_assert(handler3_count==1);
	ck_assert(handler4_count==1);
	ck_assert(handler5_count==1);

}
END_TEST


START_TEST(events_tests_broadcast3)
{
	kn_event_reg_t array[]={{
		.id=1,
		.user_data=NULL,
		.handler=event_handler1
	},{
		.id=2,
		.user_data=NULL,
		.handler=event_handler2
	},{
		.id=2,
		.user_data=NULL,
		.handler=event_handler2
	},{
		.id=3,
		.user_data=NULL,
		.handler=event_handler3
	},{
		.id=4,
		.user_data=NULL,
		.handler=event_handler4
	},{
		.id=4,
		.user_data=NULL,
		.handler=event_handler4
	},{
		.id=4,
		.user_data=NULL,
		.handler=event_handler4
	},{		
		.id=5,
		.user_data=NULL,
		.handler=event_handler5
	}};

	kn_event_t evt={};

	kn_event_reg_t map[sizeof(array)/sizeof(array[0])]={};
	int map_count=sizeof(array)/sizeof(array[0]);

	int ret=kn_eventdisp_init(&disp, map, map_count);
	ck_assert(ret==0);

	reset_states();

	ret=kn_eventdisp_subscribe_reg_array(&disp, array, sizeof(array)/sizeof(array[0]));
	ck_assert(ret==0);
	ck_assert(kn_eventdisp_get_registered_count(&disp)==sizeof(array)/sizeof(array[0]));

	evt.id=1;
	ret=kn_eventdisp_publish(&disp, &evt);
	ck_assert(ret==0);
	evt.id=2;
	ret=kn_eventdisp_publish(&disp, &evt);
	ck_assert(ret==0);
	evt.id=3;
	ret=kn_eventdisp_publish(&disp, &evt);
	ck_assert(ret==0);
	evt.id=4;
	ret=kn_eventdisp_publish(&disp, &evt);
	ck_assert(ret==0);
	evt.id=5;
	ret=kn_eventdisp_publish(&disp, &evt);
	ck_assert(ret==0);

	ck_assert(handler1_count==1);
	ck_assert(handler2_count==2);
	ck_assert(handler3_count==1);
	ck_assert(handler4_count==3);
	ck_assert(handler5_count==1);

}
END_TEST


START_TEST(events_tests_broadcast4)
{
	kn_event_reg_t array[]={{
		.id=1,
		.user_data=NULL,
		.handler=event_handler1
	},{
		.id=2,
		.user_data=NULL,
		.handler=event_handler2
	},{
		.id=2,
		.user_data=NULL,
		.handler=event_handler2
	},{
		.id=3,
		.user_data=NULL,
		.handler=event_handler3
	},{
		.id=4,
		.user_data=NULL,
		.handler=event_handler4
	},{
		.id=4,
		.user_data=NULL,
		.handler=event_handler4
	},{
		.id=4,
		.user_data=NULL,
		.handler=event_handler4
	},{		
		.id=5,
		.user_data=NULL,
		.handler=event_handler5
	}};

	kn_event_t evt={
		.id=6
	};

	kn_event_reg_t map[sizeof(array)/sizeof(array[0])]={};
	int map_count=sizeof(array)/sizeof(array[0]);

	int ret=kn_eventdisp_init(&disp, map, map_count);
	ck_assert(ret==0);

	reset_states();

	ret=kn_eventdisp_subscribe_reg_array(&disp, array, sizeof(array)/sizeof(array[0]));
	ck_assert(ret==0);
	ck_assert(kn_eventdisp_get_registered_count(&disp)==sizeof(array)/sizeof(array[0]));

	ret=kn_eventdisp_publish(&disp, &evt);
	ck_assert(ret==0);

	ck_assert(handler1_count==0);
	ck_assert(handler2_count==0);
	ck_assert(handler3_count==0);
	ck_assert(handler4_count==0);
	ck_assert(handler5_count==0);

}
END_TEST


START_TEST(events_tests_event_worker_interface)
{
	kn_event_reg_t map[6]={};
	int map_count=6;
	bool handler_called=false;

	void handler3(kn_event_t *event, void *user_data){
		ck_assert(event->id==3);
		handler_called=true;
	}


	kn_event_reg_t array[]={{
		.id=1,
		.user_data=NULL,
		.handler=dummy_handler
	},{
		.id=2,
		.user_data=NULL,
		.handler=dummy_handler
	},{
		.id=3,
		.user_data=NULL,
		.handler=handler3
	},{
		.id=4,
		.user_data=NULL,
		.handler=dummy_handler
	},{
		.id=5,
		.user_data=NULL,
		.handler=dummy_handler
	}};


	kn_event_t evt={
		.id=3
	};
	
	int ret=kn_eventdisp_init(&disp, map, map_count);
	ck_assert(ret==0);

	ret=kn_eventdisp_subscribe_reg_array(&disp, array, sizeof(array)/sizeof(array[0]));
	ck_assert(ret==0);
	ck_assert(kn_eventdisp_get_registered_count(&disp)==5);

	// Test the generic worker interface
	ret=kn_event_send_to_worker(&disp.worker, &evt);

	ck_assert(ret==0);
	ck_assert(handler_called);
}
END_TEST

// Test that events are correctly published to subecribed event workers
START_TEST(events_tests_event_worker_subscriber)
{
	bool worker_called=false;
	void *dummy_user_data=(void*)0xB10B00B5;

	int worker_on_event(kn_event_worker_t *worker, kn_event_t *event){
		ck_assert(worker!=NULL);
		ck_assert(worker->user_data==dummy_user_data);
		worker_called=true;
	}

	kn_event_reg_t map[10]={};
	int map_count=sizeof(map)/sizeof(map[0]);


	kn_event_worker_t worker={
		.on_event=worker_on_event,
		.user_data=dummy_user_data
	};
	kn_event_t evt={
		.id=6
	};

	int ret=kn_eventdisp_init(&disp, map, map_count);
	ck_assert(ret==0);

	reset_states();

	ret=kn_eventdisp_subscribe_worker(&disp, 6, &worker);
	ck_assert(ret==0);
	ck_assert(kn_eventdisp_get_registered_count(&disp)==1);

	ret=kn_eventdisp_publish(&disp, &evt);
	ck_assert(ret==0);

	ck_assert(worker_called==true);
}
END_TEST



int tests_events_dispatch_run(void){
	Suite *s1 = suite_create("Events");
	TCase *tc1_1 = tcase_create("Events");
	SRunner *sr = srunner_create(s1);
	int nf;

	suite_add_tcase(s1, tc1_1);
	tcase_add_test(tc1_1, events_tests_init);
	tcase_add_test(tc1_1, events_tests_init2);
	tcase_add_test(tc1_1, events_tests_init3);
	tcase_add_test(tc1_1, events_tests_init4);
	tcase_add_test(tc1_1, events_tests_init5);
	tcase_add_test(tc1_1, events_tests_init6);
	tcase_add_test(tc1_1, events_tests_array);
	tcase_add_test(tc1_1, events_tests_array2);
	tcase_add_test(tc1_1, events_tests_static_init);
	tcase_add_test(tc1_1, events_tests_broadcast);
	tcase_add_test(tc1_1, events_tests_broadcast2);
	tcase_add_test(tc1_1, events_tests_broadcast3);
	tcase_add_test(tc1_1, events_tests_broadcast4);
	tcase_add_test(tc1_1, events_tests_event_worker_interface);
	tcase_add_test(tc1_1, events_tests_event_worker_subscriber);

	srunner_run_all(sr, CK_ENV);
	nf = srunner_ntests_failed(sr);
	srunner_free(sr);

	return nf == 0 ? 0 : 1;
}