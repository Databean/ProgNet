#include "Script.h"
#include <SDL.h>
#include <iostream>
#include "ShapeManager.h"
#include <stdint.h>
#include "Interface.h"
#include "v8.h"

using std::cout;
using std::endl;

using v8::Handle;
using v8::ObjectTemplate;
using v8::Local;
using v8::String;
using v8::Value;
using v8::Integer;
using v8::Script;
using v8::External;
using v8::Number;
using v8::Object;
using v8::Boolean;
using v8::Context;
using v8::Array;
using v8::FunctionTemplate;
using v8::HandleScope;
using v8::Persistent;
using v8::NumberObject;
using v8::V8;
using v8::Isolate;

std::string shapeManagerStr = "__shapeManager";

GameScript::GameScript() {
	
}

GameScript::~GameScript() {
	
}

V8Script::V8Script(const std::string& program) : program(program), isolate(Isolate::New()) {
	
}

V8Script::~V8Script() {
	isolate->Dispose();
}

template<class T>
inline T externalExtract(Local<Object> item) {
	return static_cast<T>(Local<External>::Cast(item->GetInternalField(0))->Value());
}

template<class T, class S>
inline T accessorExtract(const v8::PropertyCallbackInfo<S>& info) {
	Local<Object> self = info.Holder();
	Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
	return static_cast<T>(wrap->Value());
}

static Handle<ObjectTemplate> initColor() {
	HandleScope handle_scope(Isolate::GetCurrent());
	
	auto getRed = [](Local<String> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
		info.GetReturnValue().Set(Integer::New(Isolate::GetCurrent(), accessorExtract<Color*>(info)->red()));
	};
	
	auto getGreen = [](Local<String> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
		info.GetReturnValue().Set(Integer::New(Isolate::GetCurrent(), accessorExtract<Color*>(info)->green()));
	};
	
	auto getBlue = [](Local<String> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
		info.GetReturnValue().Set(Integer::New(Isolate::GetCurrent(), accessorExtract<Color*>(info)->blue()));
	};
	
	auto setRed = [](Local<String> property, Local<Value> value, const v8::PropertyCallbackInfo<void>& info) {
		accessorExtract<Color*>(info)->red() = value->Int32Value();
	};
	
	auto setGreen = [](Local<String> property, Local<Value> value, const v8::PropertyCallbackInfo<void>& info) {
		accessorExtract<Color*>(info)->green() = value->Int32Value();
	};
	
	auto setBlue = [](Local<String> property, Local<Value> value, const v8::PropertyCallbackInfo<void>& info) {
		accessorExtract<Color*>(info)->blue() = value->Int32Value();
	};
	
	Handle<ObjectTemplate> color_templ = ObjectTemplate::New();
	color_templ->SetInternalFieldCount(1);
	color_templ->SetAccessor(String::NewFromUtf8(Isolate::GetCurrent(), "red"), getRed, setRed);
	color_templ->SetAccessor(String::NewFromUtf8(Isolate::GetCurrent(), "green"), getGreen, setGreen);
	color_templ->SetAccessor(String::NewFromUtf8(Isolate::GetCurrent(), "blue"), getBlue, setBlue);
	
	return color_templ;
}

static Handle<ObjectTemplate> initPoint() {
	HandleScope handle_scope(Isolate::GetCurrent());
	
	auto getPointX = [](Local<String> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
		info.GetReturnValue().Set(Number::New(Isolate::GetCurrent(), (*accessorExtract<Point2d*>(info))[0]));
	};
	
	auto setPointX = [](Local<String> property, Local<Value> value, const v8::PropertyCallbackInfo<void>& info) {
		(*accessorExtract<Point2d*>(info))[0] = value->NumberValue();
	};
	
	auto getPointY = [](Local<String> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
		info.GetReturnValue().Set(Number::New(Isolate::GetCurrent(), (*accessorExtract<Point2d*>(info))[1]));
	};
	
	auto setPointY = [](Local<String> property, Local<Value> value, const v8::PropertyCallbackInfo<void>& info) {
		(*accessorExtract<Point2d*>(info))[1] = value->NumberValue();
	};
	
	Handle<FunctionTemplate> func_tmpl = FunctionTemplate::New(Isolate::GetCurrent());
	Handle<ObjectTemplate> point_templ = func_tmpl->InstanceTemplate();
	point_templ->SetInternalFieldCount(1);
	point_templ->SetAccessor(String::NewFromUtf8(Isolate::GetCurrent(), "x"), getPointX, setPointX);
	point_templ->SetAccessor(String::NewFromUtf8(Isolate::GetCurrent(), "y"), getPointY, setPointY);
	
	return point_templ;
}

static Handle<ObjectTemplate> initShape() {
	HandleScope handleScope(Isolate::GetCurrent());
	
	auto getColor = [](Local<String> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
		Local<Object> self = info.Holder();
		Local<External> managerWrap = Local<External>::Cast(self->GetInternalField(0));
		Local<Integer> shapeWrap = Local<Integer>::Cast(self->GetInternalField(1));
		
		ShapeManager2d* manager = static_cast<ShapeManager2d*>(managerWrap->Value());
		Shape2d& shape = (*manager)[shapeWrap->Value()];
		
		Local<Object> obj = initColor()->NewInstance();
		obj->SetInternalField(0, External::New(Isolate::GetCurrent(), static_cast<void*>(&shape.color())));
		
		info.GetReturnValue().Set(obj);
	};
	
	auto setColor = [](Local<String> property, Local<Value> value, const v8::PropertyCallbackInfo<void>& info) {
		Local<Object> self = info.Holder();
		Local<External> managerWrap = Local<External>::Cast(self->GetInternalField(0));
		Local<Integer> shapeWrap = Local<Integer>::Cast(self->GetInternalField(1));
		Local<External> colorWrap = Local<External>::Cast(value);
		
		ShapeManager2d* manager = static_cast<ShapeManager2d*>(managerWrap->Value());
		Shape2d& shape = (*manager)[shapeWrap->Value()];
		Color* newColor = static_cast<Color*>(colorWrap->Value());
		
		shape.setColor(*newColor);
	};
	
	auto getOffset = [](Local<String> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
		Local<Object> self = info.Holder();
		Local<External> managerWrap = Local<External>::Cast(self->GetInternalField(0));
		Local<Integer> shapeWrap = Local<Integer>::Cast(self->GetInternalField(1));
		
		ShapeManager2d* manager = static_cast<ShapeManager2d*>(managerWrap->Value());
		Shape2d& shape = (*manager)[shapeWrap->Value()];
		
		Local<Object> obj = initPoint()->NewInstance();
		obj->SetInternalField(0, External::New(Isolate::GetCurrent(), static_cast<void*>(&shape.offset())));
		
		info.GetReturnValue().Set(obj);
	};
	
	auto setOffset = [](Local<String> property, Local<Value> value, const v8::PropertyCallbackInfo<void>& info) {
		Local<Object> self = info.Holder();
		Local<External> managerWrap = Local<External>::Cast(self->GetInternalField(0));
		Local<Integer> shapeWrap = Local<Integer>::Cast(self->GetInternalField(1));
		Local<External> pointWrap = Local<External>::Cast(value);
		
		ShapeManager2d* manager = static_cast<ShapeManager2d*>(managerWrap->Value());
		Shape2d& shape = (*manager)[shapeWrap->Value()];
		Point2d* newOffset = static_cast<Point2d*>(pointWrap->Value());
		
		shape.setOffset(*newOffset);
	};
	
	auto getRotation = [](Local<String> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
		Local<Object> self = info.Holder();
		Local<External> managerWrap = Local<External>::Cast(self->GetInternalField(0));
		Local<Integer> shapeWrap = Local<Integer>::Cast(self->GetInternalField(1));
		
		ShapeManager2d* manager = static_cast<ShapeManager2d*>(managerWrap->Value());
		Shape2d& shape = (*manager)[shapeWrap->Value()];
		
		info.GetReturnValue().Set(Number::New(Isolate::GetCurrent(), shape.rotation()));
	};
	
	auto setRotation = [](Local<String> property, Local<Value> value, const v8::PropertyCallbackInfo<void>& info) {
		Local<Object> self = info.Holder();
		Local<External> managerWrap = Local<External>::Cast(self->GetInternalField(0));
		Local<Integer> shapeWrap = Local<Integer>::Cast(self->GetInternalField(1));
		
		ShapeManager2d* manager = static_cast<ShapeManager2d*>(managerWrap->Value());
		Shape2d& shape = (*manager)[shapeWrap->Value()];
		
		shape.setRotation(value->NumberValue());
	};
	
	auto getPoint = [](unsigned int index, const v8::PropertyCallbackInfo<v8::Value>& info) {
		Local<Object> self = info.Holder();
		Local<External> managerWrap = Local<External>::Cast(self->GetInternalField(0));
		Local<Integer> shapeWrap = Local<Integer>::Cast(self->GetInternalField(1));
		
		ShapeManager2d* manager = static_cast<ShapeManager2d*>(managerWrap->Value());
		Shape2d& shape = (*manager)[shapeWrap->Value()];
		
		Local<Object> obj = initPoint()->NewInstance();
		obj->SetInternalField(0, External::New(Isolate::GetCurrent(), static_cast<void*>(&shape[index])));
		
		info.GetReturnValue().Set(obj);
	};
	
	auto setPoint = [](unsigned int index, Local<Value > value, const v8::PropertyCallbackInfo<v8::Value>& info) {
		Local<Object> self = info.Holder();
		Local<External> managerWrap = Local<External>::Cast(self->GetInternalField(0));
		Local<Integer> shapeWrap = Local<Integer>::Cast(self->GetInternalField(1));
		Local<External> pointWrap = Local<External>::Cast(value);
		
		ShapeManager2d* manager = static_cast<ShapeManager2d*>(managerWrap->Value());
		Shape2d& shape = (*manager)[shapeWrap->Value()];
		Point2d* newOffset = static_cast<Point2d*>(pointWrap->Value());
		
		shape.setPoint(index, *newOffset);
		
		//Returns to indicate that the access was intercepted.
		//info.GetReturnValue() = value;
	};
	
	//Workaround (i think)
	Handle<FunctionTemplate> func_tmpl = FunctionTemplate::New(Isolate::GetCurrent());
	Handle<ObjectTemplate> shape_tmpl = func_tmpl->InstanceTemplate();
	shape_tmpl->SetAccessor(String::NewFromUtf8(Isolate::GetCurrent(), "color"), getColor, setColor);
	shape_tmpl->SetAccessor(String::NewFromUtf8(Isolate::GetCurrent(), "offset"), getOffset, setOffset);
	shape_tmpl->SetAccessor(String::NewFromUtf8(Isolate::GetCurrent(), "rotation"), getRotation, setRotation);
	shape_tmpl->SetIndexedPropertyHandler(getPoint, setPoint);
	
	shape_tmpl->SetInternalFieldCount(2);
	
	//Local<Object> o = shape_tmpl->NewInstance();
	//std::cout << o->InternalFieldCount() << std::endl;
	
	return shape_tmpl;
}

/*
void getShape(const v8::FunctionCallbackInfo<v8::Value>& args) {
	if(args.Length() != 1) {
		return Boolean::New(false);
	}
	Local<External> shapeManagerExternal = Local<External>::Cast(args.Callee()->Get(String::NewFromUtf8(Isolate::GetCurrent(), shapeManagerStr.c_str())));
	ShapeManager2d* shapeManager = static_cast<ShapeManager2d*>(shapeManagerExternal->Value());
	
	unsigned int index = args[0]->NumberValue();
	Shape2d& shape = (*shapeManager)[index];
	
	Local<Object> obj = shape_tmpl->NewInstance();
	obj->SetInternalField(0, External::New(&shape));
	args.GetReturnValue().Set(obj);
}*/

/**
 * The __shapeManager field of this function should be set to the appropriate ShapeManager.
 */
void makeTriangle(const v8::FunctionCallbackInfo<v8::Value>& args) {
	if(args.Length() != 6) {
		args.GetReturnValue().Set(Boolean::New(Isolate::GetCurrent(), false));
		return;
	}
	Local<External> shapeManagerExternal = Local<External>::Cast(args.Callee()->Get(String::NewFromUtf8(Isolate::GetCurrent(), shapeManagerStr.c_str())));
	ShapeManager2d* shapeManager = static_cast<ShapeManager2d*>(shapeManagerExternal->Value());
	
	Point2d p1{args[0]->NumberValue(), args[1]->NumberValue()};
	Point2d p2{args[2]->NumberValue(), args[3]->NumberValue()};
	Point2d p3{args[4]->NumberValue(), args[5]->NumberValue()};
	
	Local<Object> obj = initShape()->NewInstance();
	unsigned int shape = shapeManager->makeTriangle(p1, p2, p3);
	obj->SetInternalField(0, shapeManagerExternal);
	obj->SetInternalField(1, Integer::NewFromUnsigned(Isolate::GetCurrent(), shape));
	
	args.GetReturnValue().Set(obj);
}

void makeLine(const v8::FunctionCallbackInfo<v8::Value>& args) {
	if(args.Length() != 4) {
		args.GetReturnValue().Set(Boolean::New(Isolate::GetCurrent(), false));
		return;
	}
	Local<External> shapeManagerExternal = Local<External>::Cast(args.Callee()->Get(String::NewFromUtf8(Isolate::GetCurrent(), shapeManagerStr.c_str())));
	ShapeManager2d* shapeManager = static_cast<ShapeManager2d*>(shapeManagerExternal->Value());
	
	Point2d p1{args[0]->NumberValue(), args[1]->NumberValue()};
	Point2d p2{args[2]->NumberValue(), args[3]->NumberValue()};
	
	Local<Object> obj = initShape()->NewInstance();
	unsigned int shape = shapeManager->makeLine(p1, p2);
	obj->SetInternalField(0, shapeManagerExternal);
	obj->SetInternalField(1, Integer::NewFromUnsigned(Isolate::GetCurrent(), shape));
	
	args.GetReturnValue().Set(obj);
}

void makeQuad(const v8::FunctionCallbackInfo<v8::Value>& args) {
	if(args.Length() != 8) {
		args.GetReturnValue().Set(Boolean::New(Isolate::GetCurrent(), false));
		return;
	}
	
	Local<External> shapeManagerExternal = Local<External>::Cast(args.Callee()->Get(String::NewFromUtf8(Isolate::GetCurrent(), shapeManagerStr.c_str())));
	ShapeManager2d* shapeManager = static_cast<ShapeManager2d*>(shapeManagerExternal->Value());
	
	Point2d p1{args[0]->NumberValue(), args[1]->NumberValue()};
	Point2d p2{args[2]->NumberValue(), args[3]->NumberValue()};
	Point2d p3{args[4]->NumberValue(), args[5]->NumberValue()};
	Point2d p4{args[6]->NumberValue(), args[7]->NumberValue()};
	
	Local<Object> obj = initShape()->NewInstance();
	unsigned int shape = shapeManager->makeQuad(p1, p2, p3, p4);
	obj->SetInternalField(0, shapeManagerExternal);
	obj->SetInternalField(1, Integer::NewFromUnsigned(Isolate::GetCurrent(), shape));
	
	args.GetReturnValue().Set(obj);
}

void frameWait(const v8::FunctionCallbackInfo<v8::Value>& args) {
	Interface::frameWait();
	args.GetReturnValue().Set(Boolean::New(Isolate::GetCurrent(), true));
}

void getMouseX(const v8::FunctionCallbackInfo<v8::Value>& args) {
	args.GetReturnValue().Set(Integer::New(Isolate::GetCurrent(), Interface::getMouseX()));
}

void getMouseY(const v8::FunctionCallbackInfo<v8::Value>& args) {
	args.GetReturnValue().Set(Integer::New(Isolate::GetCurrent(), Interface::getMouseY()));
}

void V8Script::run(ShapeManager2d& pb) {
	
	isolate->Enter();
	
	// Create a stack-allocated handle scope.
	HandleScope handle_scope(Isolate::GetCurrent());
	
	Handle<ObjectTemplate> global = ObjectTemplate::New();
	/*
	global->Set(String::NewFromUtf8(Isolate::GetCurrent(), "__buffer__"), External::New(&pb));
	
	auto color = [](const v8::FunctionCallbackInfo<v8::Value>& args) -> Handle<Value> {
		unsigned int x = args[0]->Uint32Value();
		unsigned int y = args[1]->Uint32Value();
		
		Local<Value> funcObj = args.Callee()->Get(String::NewFromUtf8(Isolate::GetCurrent(), "__buffer"));
		Local<External> wrap = Local<External>::Cast(funcObj);
		void* ptr = wrap->Value();
		
		Local<Object> obj = color_templ->NewInstance();
		obj->SetInternalField(0, External::New(&(*static_cast<PixelBuffer*>(ptr))(x, y)));
		args.GetReturnValue().Set(obj);
	};
	
	Local<FunctionTemplate> colorFuncTemplate = FunctionTemplate::New(Isolate::GetCurrent(), color);
	colorFuncTemplate->Set(String::NewFromUtf8(Isolate::GetCurrent(), "__buffer"), External::New(&pb));*/
	
	Local<External> shapeManagerWrap = External::New(Isolate::GetCurrent(), static_cast<void*>(&pb));
	
	Local<FunctionTemplate> makeTriangleTemplate = FunctionTemplate::New(Isolate::GetCurrent(), makeTriangle);
	makeTriangleTemplate->Set(Isolate::GetCurrent(), shapeManagerStr.c_str(), shapeManagerWrap);
	Local<FunctionTemplate> makeLineTemplate = FunctionTemplate::New(Isolate::GetCurrent(), makeLine);
	makeLineTemplate->Set(Isolate::GetCurrent(), shapeManagerStr.c_str(), shapeManagerWrap);
	Local<FunctionTemplate> makeQuadTemplate = FunctionTemplate::New(Isolate::GetCurrent(), makeQuad);
	makeQuadTemplate->Set(Isolate::GetCurrent(), shapeManagerStr.c_str(), shapeManagerWrap);
	
	//Local<FunctionTemplate> getShapeTemplate = FunctionTemplate::New(Isolate::GetCurrent(), getShape);
	//getShapeTemplate->Set(shapeManagerStr.c_str(), External::New(&pb));
	
	// Return an empty result if there was an error creating the array.
	/*
	if (array.IsEmpty()) {
		return Handle<Array>();
	}*/
	
	// Fill out the values
	global->Set(String::NewFromUtf8(Isolate::GetCurrent(), "makeTriangle"), makeTriangleTemplate);
	global->Set(String::NewFromUtf8(Isolate::GetCurrent(), "makeLine"), makeLineTemplate);
	global->Set(String::NewFromUtf8(Isolate::GetCurrent(), "makeQuad"), makeQuadTemplate);
	global->Set(String::NewFromUtf8(Isolate::GetCurrent(), "frameWait"), FunctionTemplate::New(Isolate::GetCurrent(), frameWait));
	global->Set(String::NewFromUtf8(Isolate::GetCurrent(), "getMouseX"), FunctionTemplate::New(Isolate::GetCurrent(), getMouseX));
	global->Set(String::NewFromUtf8(Isolate::GetCurrent(), "getMouseY"), FunctionTemplate::New(Isolate::GetCurrent(), getMouseY));
	
	//global->Set(String::NewFromUtf8(Isolate::GetCurrent(), "shape"), getShapeTemplate);
	
	// Create a new context.
	Handle<Context> context = Context::New(Isolate::GetCurrent(), NULL, global);
	
	// Enter the created context for compiling and
	// running the hello world script. 
	Context::Scope context_scope(context);
	/*
	// Create a new empty array.
	Handle<Array> array = Array::New(pb.maxX());
	
	for(unsigned int x = 0; x < pb.maxX(); x++) {
		Handle<Array> line = Array::New(pb.maxY());
		if(!line.IsEmpty()) {
			for(unsigned int y = 0; y < pb.maxY(); y++) {
				Local<Object> obj = color_templ->NewInstance();
				obj->SetInternalField(0, External::New(&pb(x, y)));
				line->Set(y, obj);
			}
		}
		array->Set(x, line);
	}
	
	context->Global()->Set(String::NewFromUtf8(Isolate::GetCurrent(), "color"), array);*/
	
	
	// Create a string containing the JavaScript source code.
	Handle<String> source = String::NewFromUtf8(Isolate::GetCurrent(), program.c_str());
	
	// Compile the source code.
	Handle<Script> script = Script::Compile(source);
	
	// Run the script to get the result.
	//while(true) {
		//Handle<Value> result = script->Run();
		script->Run();
		//SDL_Delay(1);
		//pb.clear();
	//}
	
	isolate->Exit();
}

void V8Script::terminate() {
	V8::TerminateExecution(Isolate::GetCurrent());
}

