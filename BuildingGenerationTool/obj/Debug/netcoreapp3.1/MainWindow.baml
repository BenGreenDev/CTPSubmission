   M S B A M L   `   `   `  ����Q  MBuildingGenerationTool, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null?$clr-namespace:BuildingGenerationToolBuildingGenerationTool  5   	   '    !BuildingGenerationTool.MainWindow   5   	   \ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35^ ZWindowsFormsIntegration, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35e aSystem.Windows.Controls.Ribbon, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35H 9http://schemas.microsoft.com/winfx/2006/xaml/presentation      5   	   R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5   	   @d2http://schemas.microsoft.com/expression/blend/2008     5   	   Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     5   	   6local$clr-namespace:BuildingGenerationTool     5   	     � Title$  Building Generator Tool��5   	   $	��900��6)   $
��1000��66   .��5	      �  � ColumnDefinitions 5
   
   �� 5      $��* �6    � RowDefinitions 5   
   �� 5      $��0.017* �6   �� 5      $��0.017* �6   �� 5      $��0.025* �6   6.   �� 5      $��0.0375* �6   6/   �� 5      $
��0.2* �6   6,   �� 5      $��0.02* �6   6-   .+�5   
   �� $��0��6    �� Orientation$ 
Horizontal=�6"   
����ح�$
��Left=�6R   $	��Top=�6m   $��50��6�   $
��1000��6�   .+�5      �� -   5        $��Label��5   2   $��10,0,0,0q�6   $��26��6&   $
��Bold"�6?   .��6Q   86Step 1 - Please target the miniconda install location:5      �� 5      -   5        $��MinicondaDirectory��5      $��5,0,0,0q�6.   $��25��6?   $	��300��6K   ���	#FFDEDEDE6W   �� 5      -   5        $	��...��5      $��20��6>   $��26��6I   $��5,0,0,0q�6U   5      �� 5   
   $��1��6   $ 
Horizontal=�6"   
����ح�$��50��6R   $
��Left=�6^   $��0,0,0,0q�6y   $
��1000��6�   .+�5      �� -   5        $��Label1��5   2   $��10,0,0,0q�6   $��25��6&   $
��Bold"�6@   .��6R   ,*Step 2 - Select Your Blueprint Image File:5       �� 5!      -   5        $��ImageDirectory��5!      $��5,0,0,0q�6*   $��25��6;   $	��300��6G   ���	#FFDEDEDE6S   �� 5"      -   5        $��BrowseButton��5"   2   $��5,0,0,0q�6   $��25��6&   $��75��6F   .��6l   Browse5$      5%      � 5&   
   $��2��6    � Orientation$ 
Horizontal=�6!   
����ح�$
��Left=�6Q   $��0,0,0,0q�6l   $
��1000��6}   .+�5'      �� $&�� Blueprint symbol Detection Setup��6   $��32��6?   $
��Bold"�6L   $��14(�6^   6n   �� 5(      
���  ��$c��](Warning ticking this will make processing take a long time due to analysing 1000s of images)��6%   $	��700��6�   $��Normal&�6�   6�   �� 5)      -   5        $��DetectSymbolsCheckbox��5)      $��Try Detect Symbols��64   $��26��6Q   $��	10,4,15,0q�6]   �� 5*      $"��Estimated Height Of Symbols:��6   $��32��6;   �� 5+      -   5        $��EstimatedSymbolHeight��5+      $��20��6^    �� TextWrapping$
 Wrap=�6h   $��22��6|   $��50��6�   �� 5,      $!��Estimated Width Of Symbols:��6   $��32��6:   �� 5-      -	   5        $��EstimatedSymbolWidth��5-      $	��100��6]   $
 Wrap=�6h   $��22��6|   $	��120��6�   5.      � 5/   
   $��3��6   $ 
Horizontal=�6!   
����ح�$
��Left=�6Q   $��0,0,0,0q�6l   $
��1000��6}   .+�50      �� $��Line Detection Setup��6   $��32��63   $
��1000��6?   $
��Bold"�6L   $��14(�6^   6n   �� 51      $��Min Line Length Detected:��6   $��32��68   �� 52      -
   5        $��
MinLineGap��52      $��5��6S   $
 Wrap=�6\   $��22��6p   $	��120��6|   �� 53      $��Max Line Gap Detected:��6   $��32��65   �� 54      -   5        $��
MaxLineGap��54      $��10��6S   $
 Wrap=�6]   $��22��6q   $	��120��6}   �� 55      $��Detection Threshold:��6   $��32��63   �� 56      -   5        $��DetectionThreshold��56      $��30��6[   $
 Wrap=�6e   $��22��6y   $	��120��6�   $��	0,0,150,0q�6�   �� 57      $��Min Distance To Merge:��6   $��32��65   �� 58      -   5        $��MinMergeDistance��58      $��2��6Y   $
 Wrap=�6b   $��22��6v   $	��120��6�   �� 59      $��Min Merge Angle:��6   $��32��6/   �� 5:      -   5        $��MinMergeAngle��5:      $��5��6V   $
 Wrap=�6_   $��22��6s   $	��120��6   5;      � 5=   
   $��4��6   
����ح�$ 
Horizontal=�68   $��0,0,0,0q�6Q   $��Center=�6b   $
��1000��6   .+�5>      �� $��Your Image:��6   $	��490��6*   $��10,0,0,0q�67   $��50��6I   $��20(�6U   $
��Bold"�6c   6w   �� 5?      $��Features Detected:��6   $	��500��61   $��50��6=   $��20(�6I   $
��Bold"�6W   6k   �� 5@      -   5         ��Name$ 	BaseImage��5@   P   $��Uniform=�6    �� StretchDirection$
 Both=�6&   $��10,0,0,0q�6>   $	��480��6c   $	��500��6o   �� 5A      -   5        $ GeneratedImage��5A   >   $��Uniform=�6   $
 Both=�6&   $	��480��6V   $	��500��6b   $��	10,0,10,0q�6o   5B      � 5D   
   $��5��6   
����ح�$ 
Horizontal=�68   $��0,0,0,0q�6Q   $��Center=�6b   $
��1000��6   .+�5E      �� -   5        $��	900,5,0,0q�5E      $��Detect Features��6(   $��46��6B   6g   5F      5G      5H      