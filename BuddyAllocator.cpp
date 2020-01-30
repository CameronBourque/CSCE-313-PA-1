#include "BuddyAllocator.h"
#include <iostream>
using namespace std;

BuddyAllocator::BuddyAllocator (int _basic_block_size, int _total_memory_length){
  basic_block_size = _basic_block_size, total_memory_size = _total_memory_length;
  start = (BlockHeader*)(new char[_total_memory_length]); //may do rounding up for powers of 2!!!!
}

BuddyAllocator::~BuddyAllocator (){

}

BlockHeader* BuddyAllocator::getbuddy (BlockHeader * addr){
  return (BlockHeader*) (( ((int) ( ((char*)addr) - ((char*) start)) ) ^ addr->block_size ) + (char*) start);
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
  BlockHeader* start = getbuddy(block);
  start->block_size = block->block_size >> 1;
  start->next = block->next;
  start->is_free = true;
  //Push (BlockHeader*)start into freelist

  block->block_size = block->block_size/2;
  block->next = ((BlockHeader*) start);
  return block;
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
  //FILL


  return malloc (length);
}

void BuddyAllocator::free(void* a) {
  /* Same here! */
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
    cout << "Amount of available free memory: " << total_free_memory << " byes" << endl;  
  }
}

