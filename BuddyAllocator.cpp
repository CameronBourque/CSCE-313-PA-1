#include "BuddyAllocator.h"
#include <iostream>
#include <math.h>
using namespace std;

BuddyAllocator::BuddyAllocator (int _basic_block_size, int _total_memory_length){
  basic_block_size = _basic_block_size, total_memory_size = _total_memory_length;
  total_memory_size = pow(2, (int) ceil(log2(total_memory_size))); //This is used for keeping powers of 2
  cout << "total mem size: " << total_memory_size << endl;
  basic_block_size = pow(2, (int) floor(log2(basic_block_size)));
  cout << "basic block size: " << basic_block_size << endl;
  start = (BlockHeader*)(new char[total_memory_size]);

  for(int i = (int)log2(basic_block_size); i <= (int)log2(total_memory_size); i++){
      FreeList.push_back(*(new LinkedList()));
  }

  BlockHeader *head = new BlockHeader();
  head->block_size = total_memory_size;

  FreeList[(int)log2(total_memory_size) - (int)log2(basic_block_size)].insert(head);
  cout << "inserting start mem block at index " << (int)log2(total_memory_size) - (int)log2(basic_block_size) << endl;
}

BuddyAllocator::~BuddyAllocator (){
  delete start;
  basic_block_size = 0;
  total_memory_size = 0;
}

BlockHeader* BuddyAllocator::getbuddy (BlockHeader * addr){
  return (BlockHeader*) (( ((int) ( ((char*)addr) - (char*)start) ) ^ addr->block_size ) + (char*)start);
/* IS THIS NEEDED???
  if(arebuddies(addr, ret)){
      return ret;
  }
  else{
      return NULL;
  }
  */
}

bool BuddyAllocator::arebuddies (BlockHeader* block1, BlockHeader* block2){
  return block1->block_size == block2->block_size;
}

BlockHeader* BuddyAllocator::merge (BlockHeader* block1, BlockHeader* block2){
    //do we need to clean up the space where block 2 is or is it safe to leave it to be overwritten??
    if(block1 < block2){
        block1->block_size *= 2;
        return block1;
    }
    else{
        block2->block_size *= 2;
        return block2;
    }
}

BlockHeader* BuddyAllocator::split (BlockHeader* block){
  //Moves block into new position in freelist
  block->block_size >> 1; //do this first so it finds the buddy for the split size
  BlockHeader* newBlock = getbuddy(block);
  newBlock->block_size = block->block_size;
  //Push newBlock and block into freelist (this will set its next)

  return newBlock;
}

void* BuddyAllocator::alloc(int length) {
  /* This preliminary implementation simply hands the call over the
     the C standard library!
     Of course this needs to be replaced by your implementation.
  */
  int memSpace = length + sizeof(BlockHeader);
  if(memSpace > total_memory_size)
  {
      return NULL;
  }
  
  for(int i = 0; i < FreeList.size(); i++){
      
      //FILL
  }

  return malloc (length);
}

void BuddyAllocator::free(void* a) {
  /* Same here! */
  //FILL
  ::free (a);
}

void BuddyAllocator::printlist (){
  cout << "Printing the Freelist in the format \"[index] (block size) : # of blocks\"" << endl;
  int64_t total_free_memory = 0;
  for (int i=0; i<FreeList.size(); i++){
    int blocksize = ((1<<i) * basic_block_size); // all blocks at this level are this size
    cout << "[" << i <<"] (" << blocksize << ") : ";  // block size at index should always be 2^i * bbs
    int count = 0;
    BlockHeader* b = FreeList [i].head;
    // go through the list from head to tail and count
    while (b){
      total_free_memory += blocksize;
      count ++;
      // block size at index should always be 2^i * bbs
      // checking to make sure that the block is not out of place
      if (b->block_size != blocksize){
        cerr << "ERROR:: Block is in a wrong list" << endl;
        exit (-1);
      }
      b = b->next;
    }
    cout << count << endl;
    cout << "Amount of available free memory: " << total_free_memory << " bytes" << endl;
  }
}

