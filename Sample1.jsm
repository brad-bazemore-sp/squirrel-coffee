;Sample1.jsm
.bytecode 50.0
.source Sample1.java
.class public Sample1
.super java/lang/Object
.method public <init>()V
.limit stack 1
.limit locals 1
aload_0
invokespecial java/lang/Object/<init>()V
return
.end method
.method public static main([Ljava/lang/String;)V
.limit stack 99
.limit locals 3
ldc 3
istore 0
ldc 4
istore 1
invokestatic SimpleIO/readInt()I
istore 1
ldc 9
iload 1
ldc 8
imul
iadd
istore 0
ldc "Result is "
invokestatic SimpleIO/printString(Ljava/lang/String;)V
iload 0
invokestatic SimpleIO/printInt(I)V
invokestatic SimpleIO/println()V
return
.end method
.method static <clinit>()V
.limit stack 1
.limit locals 0
return
.end method