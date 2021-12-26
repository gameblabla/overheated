#ifdef DREAMCAST

#include <kos.h>
#include <zlib/zlib.h> //vmu compression!
#include "dreamcast_icon.h"

int DC_LoadVMU(char* filetoload, char* RAM) {

    unsigned long unzipsize;
    uint8* unzipdata;
    vmu_pkg_t pkg;
	char tm[64];

    // Name of the file to open
    file_t fd;
    /*for(i=1;i<4;i++)
    {
		snprintf(tm, sizeof(tm), "/vmu/a%d/%s", i, namevmu_save);
		if (fd = fs_open(tm, O_RDONLY))
		{
			breakout = 0;
			break;
		}
	}*/
	sprintf(tm, "/vmu/a1/%s", filetoload);
	if ((fd = fs_open(tm, O_RDONLY)) == -1)
	{
		printf("error load save from VMUs, not found.\n");
		return -1;
	}
    
    // Remove VMU header
    vmu_pkg_parse(fs_mmap(fd), &pkg);

    // Allocate memory for the uncompressed data
    unzipdata = (uint8 *)malloc(524288); //512KB
    unzipsize = 524288;
    uncompress(unzipdata, &unzipsize, (uint8 *)pkg.data, pkg.data_len);

    // Save buffer into a RAM file
    fs_close(fd);

    file_t fd2;
    if ((fd2 = fs_open(RAM, O_WRONLY)) == -1)
    {
        printf("Can't create RAM file from VMU.\n");
        return -1;
    }
    fs_write(fd2, unzipdata, unzipsize);
    fs_close(fd2);

    // Free unused memory
    free(unzipdata);

    return 0;
}

int DC_SaveVMU(char* filetosave, char* namevmu_save, char* description)
{
    vmu_pkg_t pkg;
    uint8 *pkg_out;
    int pkg_size;
    file_t ft;
    char tm[64];
    
    #ifdef DEBUG
    printf("SaveVMU\n");
    #endif

    // Temporal for reading the file
    file_t file;
    int data_size;
    unsigned long zipsize = 0;
    uint8 *datasave;
    uint8 *zipdata;
	
    // Open file and copy to buffer
    file = fs_open(filetosave, O_RDONLY);
    data_size = fs_total(file);
    datasave = (uint8 *)malloc(data_size+1);
    fs_read(file, datasave, data_size);
    fs_close(file);
    
    // Allocate some memory for compression
    zipsize = data_size * 2;
    zipdata = (uint8*)malloc(zipsize);

    // The compressed save
    compress(zipdata, &zipsize, datasave, data_size);

    // Make the package to the VMU.
    strcpy(pkg.desc_short, description);
    strcpy(pkg.desc_long, "OverHeated");
    strcpy(pkg.app_id, "overheated");
    pkg.icon_cnt = 1;
    memcpy((void *)&pkg.icon_pal[0],(void *)&vmu_savestate_pal,32);
    pkg.icon_data = (const uint8*)&vmu_savestate_data;
    pkg.icon_anim_speed = 0;
    pkg.eyecatch_type = VMUPKG_EC_NONE;
    pkg.data_len = zipsize;
    pkg.data = zipdata;
    vmu_pkg_build(&pkg, &pkg_out, &pkg_size);

    // Write at A1, A2, A3, A4 port
    // /vmu/a1/overheated 
    /*for(i=1;i<4;i++)
    {
		snprintf(tm, sizeof(tm), "/vmu/a%d/%s", i, namevmu_save);
		fs_unlink(tm);
		ft = fs_open(tm, O_WRONLY);
		if (ft)
		{
			breakout = 0;
			#ifdef DEBUG
			printf("VMU %d found save \n", i);
			#endif
			break;
		}
	}*/
	sprintf(tm, "/vmu/a1/%s", namevmu_save);
	fs_unlink(tm);
	ft = fs_open(tm, O_WRONLY);
    if (!ft) {
		printf("Couldn't write save to any VMU\n");
        return -1;
    }

    fs_write(ft, pkg_out, pkg_size);
    fs_close(ft);
    
	#ifdef DEBUG
	printf("Path : %s\n", tm);
	printf("NameVMU : %s\n", namevmu_save);
	printf("File written ? \n");
	#endif

    // Free unused memory
    free(pkg_out);
    free(datasave);
    free(zipdata);
	
    return 0;
}
#endif
