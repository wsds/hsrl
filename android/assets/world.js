
{
	"A" :
	{
		properties :
		{
			size : "800*600*200",
			position : "200,100,0"
		},
		"a" :
		{
			properties :
			{
				image : "image1.jpg",
				size : "300*6000*0",
				position : "100,100,0",
				links :
				{
					TouchEventMove :
					{
						active : "true",
						factor : "0*1*1"
					}
				},
				interpolators :
				{
					deceleration :
					{
						active : "false",
						factor : "0*1.5*1"
					}
				}
			}
		},
		"b" :
		{
			properties :
			{
				image : "emoji_click.png",
				size : "96*96*0",
				position : "100,200,0",
				links :
				{
					TouchEventMove :
					{
						active : "false",
						factor : "1.5*0.5*1"
					}
				}
			}
		},
		"c" :
		{
			properties :
			{
				image : "image1.jpg",
				size : "400*300*0",
				position : "300,400,0"

			},
			"c1" :
			{
				properties :
				{
					image : "emoji_normal.png",
					size : "96*96*0",
					position : "20,180,0",
					links :
					{
						TouchEventMove :
						{
							active : "true",
							factor : "1*0.0*1"
						}
					},
					interpolators :
					{
						deceleration :
						{
							active : "false",
							factor : "1.5*0.0*1"
						}
					}
				},
				"c11" :
				{
					properties :
					{
						image : "image2.jpg",
						size : "6000*450*0",
						position : "20,100,0"
					}

				}
			}
		}
	},
	"B" :
	{
		properties :
		{
			size : "800*600*200",
			position : "200,100,0"
		},
		"a" :
		{
			properties :
			{
				image : "emoji_1.png",
				size : "96*96*0",
				position : "200,200,0",
				links :
				{
					TouchEventMove :
					{
						active : "false",
						factor : "1*1*1"
					}
				},
				interpolators :
				{
					deceleration :
					{
						active : "false"
					}
				}
			}
		}
	}
}
