import json
import sys
from typing import List, Dict, Optional
from pydantic import BaseModel, Field, validator, RootModel

# --- Schema Definitions ---

class Loot(BaseModel):
    name: str = Field(..., min_length=1)
    value: int = Field(..., ge=0)

class Monster(BaseModel):
    id: int = Field(..., gt=0)
    name: str = Field(..., min_length=1)
    hp: int = Field(..., gt=0)
    def_: int = Field(..., alias="def", ge=0)
    is_boss: int = Field(..., ge=0, le=1)
    loot: Loot

class Dungeon(BaseModel):
    name: str = Field(..., min_length=1)
    min_cp: int = Field(..., ge=0)
    max_cp: int = Field(..., ge=0)
    monster_ids: List[int]
    has_boss: int = Field(..., ge=0, le=1)
    boss_id: Optional[int] = None

    @validator('max_cp')
    def max_cp_must_be_greater_than_min(cls, v, values):
        if 'min_cp' in values and v < values['min_cp']:
            raise ValueError('max_cp must be greater than min_cp')
        return v

# --- Validation Logic ---

def validate_all():
    errors = []
    
    try:
        # 1. Load and validate monsters
        with open('data/monsters.json', 'r', encoding='utf-8') as f:
            monsters_raw = json.load(f)
        
        monsters = []
        monster_id_map = {}
        for m_data in monsters_raw:
            m = Monster(**m_data)
            monsters.append(m)
            monster_id_map[m.id] = m.name

        print(f"✓ Verified {len(monsters)} monsters.")

        # 2. Load and validate dungeons
        with open('data/dungeons.json', 'r', encoding='utf-8') as f:
            dungeons_raw = json.load(f)
        
        for d_data in dungeons_raw:
            d = Dungeon(**d_data)
            
            # Cross-reference check: Do monster IDs exist?
            for mid in d.monster_ids:
                if mid not in monster_id_map:
                    errors.append(f"Dungeon '{d.name}' references non-existent monster ID: {mid}")
            
            if d.has_boss and d.boss_id and d.boss_id not in monster_id_map:
                errors.append(f"Dungeon '{d.name}' references non-existent boss ID: {d.boss_id}")

        print(f"✓ Verified {len(dungeons_raw)} dungeons.")

    except Exception as e:
        print(f"❌ Validation Error: {e}")
        sys.exit(1)

    if errors:
        for err in errors:
            print(f"❌ Business Logic Error: {err}")
        sys.exit(1)
    
    print("✨ All data validation passed successfully!")

if __name__ == "__main__":
    validate_all()
