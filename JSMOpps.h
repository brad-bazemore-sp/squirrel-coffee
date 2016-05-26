//
// Created by brad on 5/7/16.
//

#ifndef JSMOPPS_H
#define JSMOPPS_H


//###################################################################
//Storage of values and conversion
//###################################################################

/**
 *
 */
std::string null_decl(std::string index){
    return "aconst_null\nastore "+index+"\n";
}

/**
 * load constant onto the stack
 */
std::string ldc(std::string value) {
    return "ldc " + value + "\n";
}

/**
 * load a null onto the stack
 */
std::string aconst_null(){
    return "aconst_null\n";
}

/**
 * store int value into variable #index
 */
std::string istore(std::string index) {
    return "istore " + index + "\n";
}

/**
 * store a float value into a local variable #index
 */
std::string fstore(std::string index) {
    return "fstore " + index + "\n";
}

/**
 * load an int value from a local variable #index
 */
std::string iload(std::string index) {
    return "iload " + index + "\n";
}

/**
 * load a float value from a local variable #index
 */
std::string fload(std::string index) {
    return "fload " + index + "\n";
}

/**
 * get a static field value of a class,
 * where the field is identified by field reference in the constant pool index (index1 << 8 + index2)
 */
std::string getstatic(std::string filed, std::string type){
    return "getstatic "+filed+" "+type+"\n";
}

/**
 * set static field to value in a class,
 * where the field is identified by a field reference index in constant pool (indexbyte1 << 8 + indexbyte2)
 */
std::string putstatic(std::string field){
    return "putstatic "+field+"\n";
}

/**
 * convert an int into a float
 */
std::string i2f(){
    return "i2f\n";
}

/**
 * convert a float to an int
 */
std::string f2i(){
    return "f2i\n";
}

//###################################################################
//Arithmetic operators
//###################################################################


/**
 * add two ints
 */
std::string iadd() {
    return "iadd\n";
}

/**
 * add two floats
 */
std::string fadd() {
    return "fadd\n";
}

/**
 * subtract two ints
 */
std::string isub() {
    return "isub\n";
}

/**
 * subtract two floats
 */
std::string fsub() {
    return "fsub\n";
}

/**
 * multiply two ints
 */
std::string imul() {
    return "imul\n";
}

/**
 * multiply two floats
 */
std::string fmul() {
    return "fmul\n";
}

/**
 * divide two ints
 */
std::string idiv() {
    return "idiv\n";
}

/**
 * divide two floats
 */
std::string fdiv() {
    return "fdiv\n";
}

/**
 * negate int
 */
std::string ineg() {
    return "ineg\n";
}

/**
 * negate float
 */
std::string fneg() {
    return "fneg\n";
}


//###################################################################
//Branch control
//###################################################################


/**
 * goes to another instruction at branchoffset
 * (signed short constructed from unsigned bytes branchbyte1 << 8 + branchbyte2)
 */
std::string jsm_goto(std::string label){
    return "goto "+label+"\n";
}

//###########################
//Controls for integers
//###########################

/**
 * if ints are equal, branch to instruction at branchoffset
 * (signed short constructed from unsigned bytes branchbyte1 << 8 + branchbyte2)
 */
std::string if_icmpeq(std::string else_label) {
    return "if_icmpeq " + else_label + "\n";
}

/**
 * if ints are not equal, branch to instruction at branchoffset
 * (signed short constructed from unsigned bytes branchbyte1 << 8 + branchbyte2)
 */
std::string if_icmpne(std::string else_label) {
    return "if_icmpne " + else_label + "\n";
}

/**
 * if value1 is greater than or equal to value2, branch to instruction at branchoffset
 * (signed short constructed from unsigned bytes branchbyte1 << 8 + branchbyte2)
 */
std::string if_icmpgt(std::string else_label) {
    return "if_icmpgt " + else_label + "\n";
}

/**
 * if value1 is less than value2, branch to instruction at branchoffset
 * (signed short constructed from unsigned bytes branchbyte1 << 8 + branchbyte2)
 */
std::string if_icmplt(std::string else_label) {
    return "if_icmplt " + else_label + "\n";
}

/**
 * if value1 is greater than or equal to value2, branch to instruction at branchoffset
 * (signed short constructed from unsigned bytes branchbyte1 << 8 + branchbyte2)
 */
std::string if_icmpge(std::string else_label) {
    return "if_icmple " + else_label + "\n";
}

/**
 * if value1 is less than or equal to value2, branch to instruction at branchoffset
 * (signed short constructed from unsigned bytes branchbyte1 << 8 + branchbyte2)
 */
std::string if_icmple(std::string else_label) {
    return "if_icmple " + else_label + "\n";
}

//###########################
//Controls for floats
//###########################

/**
 * compare two floats
 */
std::string fcmpg() {
    return "fcmpg\n";
}

/**
 * if value is not 0, branch to instruction at branchoffset
 * (signed short constructed from unsigned bytes branchbyte1 << 8 + branchbyte2)
 */
std::string ifeq(std::string else_label) {
    return "ifeq " + else_label + "\n";
}

/**
 * if value is not 0, branch to instruction at branchoffset
 * (signed short constructed from unsigned bytes branchbyte1 << 8 + branchbyte2)
 */
std::string ifne(std::string else_label){
    return "ifne "+else_label+"\n";
}

/**
 * if value is greater than 0, branch to instruction at branchoffset
 * (signed short constructed from unsigned bytes branchbyte1 << 8 + branchbyte2)
 */
std::string ifgt(std::string else_label){
    return "ifgt "+else_label+"\n";
}

/**
 * if value is less than 0, branch to instruction at branchoffset
 * (signed short constructed from unsigned bytes branchbyte1 << 8 + branchbyte2)
 */
std::string iflt(std::string else_label){
    return "iflt "+else_label+"\n";
}

/**
 * if value is greater than or equal to 0, branch to instruction at branchoffset
 * (signed short constructed from unsigned bytes branchbyte1 << 8 + branchbyte2)
 */
std::string ifge(std::string else_label){
    return "ifge "+else_label+"\n";
}

/**
 * if value is less than or equal to 0, branch to instruction at branchoffset
 * (signed short constructed from unsigned bytes branchbyte1 << 8 + branchbyte2)
 */
std::string ifle(std::string else_label){
    return "ifle "+else_label+"\n";
}


//###################################################################
//Method operators
//###################################################################


/**
 * invoke a static method and puts the result on the stack (might be void);
 * the method is identified by method reference index in constant pool (indexbyte1 << 8 + indexbyte2)
 */
std::string invokestatic(std::string method_desc){
    return "invokestatic "+method_desc+"\n";
}

/**
 * return void from method
 */
std::string retur() {
    return "return\n";
}

/**
 * return an integer from a method
 */
std::string ireturn() {
    return "ireturn\n";
}

/**
 * return a float from a method
 */
std::string freturn() {
    return "freturn\n";
}


#endif //TJC_PART_2_JVMOPPS_H
